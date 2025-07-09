/**
  ******************************************************************************
  * @file    iq_data_converter.c
  * @brief   IQ data converter implementation for SC12 to float format
  *          Processes 152-byte radar packets from UART queue
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iq_data_converter.h"
#include "uart.h"
#include <string.h>

/* Private function prototypes -----------------------------------------------*/
static void IQ_PrintDebug(const char* message, uint32_t value);
static void IQ_AnalyzeIncompletePacket(CircularQueue *input_queue);

/* Public functions ----------------------------------------------------------*/
/**
  * @brief  Validate radar packet in queue (check for start/end bytes)
  * @param  input_queue: Circular queue containing radar data
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ValidateRadarPacket(CircularQueue *input_queue)
{
    if (input_queue == NULL) {
        return HAL_ERROR;
    }
    
    // Check if we have enough data for a complete packet
    if (queue_size(input_queue) < RADAR_PACKET_SIZE) {
        return HAL_ERROR;  // Not enough data
    }
    
    // Find start byte
    if(queue_peek(input_queue) != RADAR_START_BYTE) {
        // IQ_PrintDebug("Start byte not found at head", queue_peek(input_queue));
        return HAL_ERROR;
    }

    
    return HAL_OK;
}

/**
  * @brief  Convert SC12 data from UART queue to I/Q queues
  *         Reads 152-byte packet, converts to float32, stores in separate I/Q queues
  * @param  input_queue: Input UART circular queue with radar data
  * @param  i_queue: Output I component queue
  * @param  q_queue: Output Q component queue
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ConvertQueueToIQQueues(CircularQueue *input_queue, FloatQueue *i_queue, FloatQueue *q_queue)
{
    if (input_queue == NULL || i_queue == NULL || q_queue == NULL) {
        return HAL_ERROR;
    }
    
    // Check if we have enough data for complete packet
    if (queue_size(input_queue) < RADAR_PACKET_SIZE) {
        // IQ_PrintDebug("Not enough data in UART queue", queue_size(input_queue));
        
        // Debug: Show what we have in the queue when size is close to expected
        if (queue_size(input_queue) >= 140) {
            char debug_msg[100];
            // sprintf(debug_msg, "[IQ_CONV] Queue size %lu, expected %d, missing %d bytes\r\n", 
            //         queue_size(input_queue), RADAR_PACKET_SIZE, 
            //         RADAR_PACKET_SIZE - queue_size(input_queue));
            // UART_Send_String(debug_msg);
            
            // Show first few bytes to check for start byte
            if (queue_size(input_queue) > 0) {
                // sprintf(debug_msg, "[IQ_CONV] First byte: 0x%02X (expected 0x%02X)\r\n", 
                //         queue_peek(input_queue), RADAR_START_BYTE);
                // UART_Send_String(debug_msg);
            }
        }
        
        return HAL_ERROR;
    }
    
    // Read and validate start byte
    uint8_t start_byte = queue_dequeue(input_queue);
    if (start_byte != RADAR_START_BYTE) {
        // IQ_PrintDebug("Invalid start byte", start_byte);
        return HAL_ERROR;
    }
    
    for (uint32_t sample_idx = 0; sample_idx < IQ_SAMPLES_PER_PACKET; sample_idx++) {
        // Check if we have enough bytes for this sample (3 bytes)
        if (queue_size(input_queue) < 3) {
            // IQ_PrintDebug("UART queue underrun during conversion", sample_idx);
            return HAL_ERROR;
        }
        
        // Read 3 bytes for one I/Q sample
        uint8_t data0 = queue_dequeue(input_queue);
        uint8_t data1 = queue_dequeue(input_queue);
        uint8_t data2 = queue_dequeue(input_queue);
        
        // Convert SC12 to SC16 using Python logic:
        // I = (data[0]<<4) | (data[1]>>4)
        // Q = ((data[1]&0xf)<<8) | data[2]
        uint16_t i_value = IQ_SC12_TO_SC16_I(data0, data1);
        uint16_t q_value = IQ_SC12_TO_SC16_Q(data1, data2);
        
        // Convert to float with proper scaling
        float_queue_enqueue(i_queue, IQ_SC16_TO_FLOAT(i_value));
        float_queue_enqueue(q_queue, IQ_SC16_TO_FLOAT(q_value));

    }
    
    // Read and validate end byte
    if (queue_is_empty(input_queue)) {
        // IQ_PrintDebug("UART queue empty, no end byte", 0);
        return HAL_ERROR;
    }
    
    uint8_t end_byte = queue_dequeue(input_queue);
    if (end_byte != RADAR_END_BYTE) {
        // IQ_PrintDebug("Invalid end byte", end_byte);
        return HAL_ERROR;
    }
    
    //Debug information
    // IQ_PrintDebug("I float queue count after conversion", float_queue_count(i_queue));
    // IQ_PrintDebug("Q float queue count after conversion", float_queue_count(q_queue));
    
    return HAL_OK;
}

/**
  * @brief  Check if I/Q float queues have enough data for signal processing
  *         Each queue should have 125 float samples
  * @param  i_queue: I component float queue
  * @param  q_queue: Q component float queue
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_CheckIQQueuesReady(FloatQueue *i_queue, FloatQueue *q_queue)
{
    if (i_queue == NULL || q_queue == NULL) {
        return HAL_ERROR;
    }
    
    // Check if both queues have 125 float samples (for signal processing)
    if (float_queue_count(i_queue) >= IQ_SAMPLES_FOR_PROCESSING && 
        float_queue_count(q_queue) >= IQ_SAMPLES_FOR_PROCESSING) {
        // IQ_PrintDebug("I/Q float queues ready for processing", float_queue_count(i_queue));
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Print debug information
  * @param  message: Debug message
  * @param  value: Value to print
  * @retval None
  */
static void IQ_PrintDebug(const char* message, uint32_t value)
{
    char debug_buffer[100];
    sprintf(debug_buffer, "[IQ_CONV] %s: %lu\r\n", message, value);
    UART_Send_String(debug_buffer);
}

/**
  * @brief  Analyze incomplete packet (149 bytes case)
  * @param  input_queue: Input queue to analyze
  * @retval None
  */
static void IQ_AnalyzeIncompletePacket(CircularQueue *input_queue)
{
    if (input_queue == NULL || queue_size(input_queue) == 0) {
        return;
    }
    
    char debug_msg[120];
    uint32_t size = queue_size(input_queue);
    
    sprintf(debug_msg, "[IQ_CONV] Analyzing incomplete packet - Size: %lu\r\n", size);
    UART_Send_String(debug_msg);
    
    // Check if we can find start byte at current position
    uint8_t first_byte = queue_peek(input_queue);
    sprintf(debug_msg, "[IQ_CONV] First byte: 0x%02X (start byte: 0x%02X)\r\n", 
            first_byte, RADAR_START_BYTE);
    UART_Send_String(debug_msg);
    
    // If we have at least 149 bytes, check what's at position 148 (potential end byte)
    if (size >= 149) {
        // We need to peek at position 148 (0-indexed), but queue_peek only shows position 0
        // So we'll create a temporary copy to check
        sprintf(debug_msg, "[IQ_CONV] 149 bytes available - checking for missing bytes\r\n");
        UART_Send_String(debug_msg);
        
        // Suggest possible solutions
        if (first_byte == RADAR_START_BYTE) {
            sprintf(debug_msg, "[IQ_CONV] Start byte OK - probably missing 3 bytes at end\r\n");
        } else {
            sprintf(debug_msg, "[IQ_CONV] Start byte wrong - packet may be shifted\r\n");
        }
        UART_Send_String(debug_msg);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*
=== 사용 예제 (새로운 I/Q 큐 기반 처리) ===

1. 초기화 (main.c에서):
   
   // I/Q 큐 선언
   CircularQueue i_data_queue;
   CircularQueue q_data_queue;
   CircularQueue uart2_rx_queue;
   
   // 초기화
   queue_init(&uart2_rx_queue);
   IQ_Converter_Init(&i_data_queue, &q_data_queue);

2. 메인 루프에서 데이터 처리:
   
   // 1초마다 5번 처리되는 루프에서
   while (1) {
       // UART2 큐에 152바이트 레이더 패킷이 수신되었을 때 (1초에 5번)
       if (queue_size(&uart2_rx_queue) >= RADAR_PACKET_SIZE) {
           // UART 데이터를 I/Q 큐로 변환
           if (IQ_ConvertQueueToIQQueues(&uart2_rx_queue, &i_data_queue, &q_data_queue) == HAL_OK) {
               printf("Packet converted successfully\n");
           }
       }
       
       // I/Q 큐가 500바이트씩 쌓이면 신호처리 수행
       if (IQ_CheckIQQueuesReady(&i_data_queue, &q_data_queue) == HAL_OK) {
           // 500바이트씩 I/Q 데이터가 준비됨 → 신호처리 수행
           // 이 부분은 main.c에서 iq_signal_processing 함수 호출
           
           // 예: 500바이트 = 125 샘플의 float32 데이터 처리
           float32_t i_data[125];  // 500/4 = 125 samples
           float32_t q_data[125];
           
           // I/Q 큐에서 데이터 추출 (바이트를 float32로 변환)
           for (int i = 0; i < 125; i++) {
               uint8_t i_bytes[4], q_bytes[4];
               for (int j = 0; j < 4; j++) {
                   i_bytes[j] = queue_dequeue(&i_data_queue);
                   q_bytes[j] = queue_dequeue(&q_data_queue);
               }
               i_data[i] = *((float32_t*)i_bytes);
               q_data[i] = *((float32_t*)q_bytes);
           }
           
           // 신호 품질 계산
           SignalQuality_t quality;
           if (IQ_CalculateSignalQuality(i_data, q_data, &quality) == HAL_OK) {
               printf("Phase STD: %.6f, Quality: %.2f\n", 
                      quality.phase_std, quality.final_quality);
           }
       }
       
       HAL_Delay(200);  // 0.2초 대기 (1초에 5번 실행)
   }

=== 새로운 데이터 흐름 ===

1. 레이더 → UART2 → uart2_rx_queue (152바이트 패킷, 1초에 5번)
2. IQ_ValidateRadarPacket() → 패킷 크기 및 start 바이트 확인
3. IQ_ConvertQueueToIQQueues() → UART 큐에서 I/Q 큐로 변환:
   - 152바이트 → 150바이트 → 50샘플 → 200바이트 float32 → 100바이트씩 I/Q 큐
4. 5번 반복 후 I/Q 큐 각각 500바이트 누적
5. IQ_CheckIQQueuesReady() → 500바이트 확인 후 신호처리
6. IQ_CalculateSignalQuality() → 신호 품질 계산

=== 메모리 사용량 ===

- i_data_queue: MAX_QUEUE_SIZE (1000 bytes) - 실제 사용 500 bytes
- q_data_queue: MAX_QUEUE_SIZE (1000 bytes) - 실제 사용 500 bytes  
- uart2_rx_queue: MAX_QUEUE_SIZE (1000 bytes) - 임시 저장용
- temp_i_buffer: 50 * 4 = 200 bytes
- temp_q_buffer: 50 * 4 = 200 bytes
- 총합: 약 3400 bytes (3.4KB)

=== 타이밍 ===

- 1초당 5번 UART 데이터 수신 (152바이트 × 5 = 760바이트)
- 1초당 I/Q 큐에 누적: 200바이트 × 5 = 1000바이트 (500바이트씩 I, Q)
- 1초마다 신호처리 1번 수행 (500바이트 = 125샘플)

=== 개선 사항 ===

1. 버퍼 효율성: 전역 버퍼 대신 큐 기반 처리
2. 확장성: 큐 크기 조절로 처리량 조절 가능
3. 실시간성: 패킷별 즉시 변환, 일정량 누적 후 처리
4. 메모리 관리: 큐 오버플로우 방지 로직 포함
5. 모듈화: iq_processor 제거로 main.c에서 직접 제어

=== Python 코드 대응 (동일) ===

Python 원본:
    I = (data[0]<<4) | (data[1]>>4)
    Q = ((data[1]&0xf)<<8) | data[2]

C 구현:
    uint16_t i_value = IQ_SC12_TO_SC16_I(data0, data1);
    uint16_t q_value = IQ_SC12_TO_SC16_Q(data1, data2);
    temp_i_buffer[i] = IQ_SC16_TO_FLOAT32(i_value);
    temp_q_buffer[i] = IQ_SC16_TO_FLOAT32(q_value);
*/
