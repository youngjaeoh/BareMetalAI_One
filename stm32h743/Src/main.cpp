/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "uart.h"
#include "lcd.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>

#include "thread_spi.h"
// Data Format
#include "queue.h"
#include "float_queue.h"
#include "queue_10.h"
//IO
#include "buzzer.h"
#include "data_trans_receive.h"
// Radar
#include "iq_data_converter.h"
#include "iq_signal_processing.h"
#include "movement_detection.h"

// TensorFlow Lite
#include "tensorflow/lite/core/c/common.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_profiler.h"

// Model
#include "models/models_tflite_model_qat_model_tflite.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// IoT 명령 상수들
#define IOT_CMD_LIGHT_ON     "light on"
#define IOT_CMD_LIGHT_OFF    "light off"
#define IOT_CMD_AC_ON        "airconditioner on"
#define IOT_CMD_AC_OFF       "airconditioner off"
#define IOT_CMD_TV_ON        "tv on"
#define IOT_CMD_TV_OFF       "tv off"
#define IOT_CMD_SPEAKER_ON   "speaker on"
#define IOT_CMD_SPEAKER_OFF  "speaker off"
#define IOT_CMD_ALL_ON       "all on"
#define IOT_CMD_ALL_OFF      "all off"

// 추가 메시지 타입 상수들
#define MSG_ALARM_FLAG_ON    0x05
#define MSG_WAKE_UP          0x06
#define MSG_SNOOZE           0x07
#define MSG_STANDBY_READY    0x08

// 알람 관련 상수들
#define WAKE_UP_TIMEOUT      10000  // 10초 후 기상으로 간주

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CircularQueue uart2_rx_queue; // UART Data Buffer
FloatQueue i_data_queue; // I Data Queue
FloatQueue q_data_queue; // Q Data Queue
ProbQueue prob_queue; // 최근 10개 예측 확률을 저장하는 큐

// Main Loop Variables
enum wakeup_mode { SLEEP_MODE, WAKEUP_MODE, FRENZY_MODE } current_mode = SLEEP_MODE; // false : sleep mode, true : wakeup mode
bool alarm_flag = true; // alarm flag
uint32_t wakeup_time = 10000; // 10 seconds
enum user_state {UNKNOWN, SLEEP, AWAKE} current_user_state = UNKNOWN; // 현재 사용자 상태
int awake_count = 0; // 깨어있는 시간 카운트

// 새로운 알람 관련 변수들
bool alarm_sent = false; // 알람 플래그 전송 완료 여부
bool snooze_flag = false; // 스누즈 플래그
uint32_t test_counter = 0; // 테스트 카운터

// Tensor arena - 메모리 부족 문제 해결을 위해 크기 증가
constexpr int tensor_arena_size = 1024*4;  // 1KB -> 4KB로 증가
static uint8_t tensor_arena[tensor_arena_size];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

#ifdef TFT96
// QQVGA
#define FrameWidth 160
#define FrameHeight 120
#elif TFT18
// QQVGA2
#define FrameWidth 128
#define FrameHeight 160
#endif

// IoT 제어 함수들
void IoT_SendCommand(const char* command);

// TensorFlow Lite 함수들
int GetSmoothedPrediction(const float* new_probs, ProbQueue* queue);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct = {0};

	/* Disables the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes for the QSPI 256MB without instruction access */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress      = QSPI_BASE;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_256MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes for the QSPI 8MB (QSPI Flash Size) to Cacheable WT */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER1;
	MPU_InitStruct.BaseAddress      = QSPI_BASE;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_8MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Setup AXI SRAM in Cacheable WB */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = D1_AXISRAM_BASE;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enables the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void CPU_CACHE_Enable(void)
{
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

void LED_Blink(uint32_t Hdelay, uint32_t Ldelay)
{
	HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_SET);
	HAL_Delay(Hdelay - 1);
	HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_RESET);
	HAL_Delay(Ldelay - 1);
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	MPU_Config();
	CPU_CACHE_Enable();
	/* USER CODE END 1 */
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_SPI4_Init();
	MX_TIM1_Init();
	LCD_Test();
	MX_DMA_Init();
	MX_SPI3_Init();
	UART_Init();

	// 중요: IQ 신호처리 모듈 초기화 - 이게 없으면 HARD_FAULT 발생!
	IQ_SignalProcessing_Init();
	
	// 중요: Movement detection 초기화
	Movement_Detection_Init();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* USER CODE BEGIN 2 */

	uint8_t text[32];
	// Initialize UART RX Queues
	queue_init(&uart2_rx_queue);
	float_queue_init(&i_data_queue);
	float_queue_init(&q_data_queue);
	// Initialize ProbQueue for storing recent predictions
	ProbQueue_Init(&prob_queue);

	//model initialization
	const tflite::Model* model = ::tflite::GetModel(models_tflite_model_qat_model_tflite);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		for(;;);
	}
	
	tflite::MicroMutableOpResolver<4> micro_op_resolver;

	micro_op_resolver.AddDequantize();
	micro_op_resolver.AddFullyConnected();
	micro_op_resolver.AddQuantize();
	micro_op_resolver.AddSoftmax();

	tflite::MicroInterpreter interpreter(model, micro_op_resolver, tensor_arena, tensor_arena_size);
	
	sprintf((char *)&text, "Tensor Arena Size: %d bytes\n\r", tensor_arena_size);
	UART_Send_String((char*)text);
	
	if(interpreter.AllocateTensors() != kTfLiteOk){
		sprintf((char *)&text, "TFLM Allocate Tensor Failed!!");
		UART_Send_String((char*)text);
		
		// 추가 디버그 정보
		sprintf((char *)&text, "Arena used: %d bytes\n\r", interpreter.arena_used_bytes());
		UART_Send_String((char*)text);
		while(1);
	}

	sprintf((char *)&text, "Tensor Arena used : %d\n\r", interpreter.arena_used_bytes());
	UART_Send_String((char*)text);
	TfLiteTensor* input = interpreter.input(0);
	sprintf((char *)&text, "Input_Shape : (%d,%d)\n\r", input->dims->data[0], input->dims->data[1]) ;
	UART_Send_String((char*)text);
	sprintf((char *)&text, "Input_Size : %d elements\n\r", input->dims->data[1]);
	UART_Send_String((char*)text);


	#ifdef USE_UART3
	// Initialize Data Transmission/Reception module
	DataTransReceive_Init(BOARD_A);  // Change to BOARD_B for slave board
	SimplePacket_t init_packet = {0};
	// Test mode selection
	uint8_t test_mode = 0; // 0: Ping-Pong test, 1: Flag-based data transmission
	uint32_t mode_switch_time = HAL_GetTick();
	#endif
	// uint32_t start_time = HAL_GetTick();  // 사용하지 않으므로 주석 처리
	// Thread_SPI_Packet_t received_packet;  // 사용하지 않으므로 주석 처리

	ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width, ST7735Ctx.Height, BLACK);

	auto clearLine = [](uint16_t y, uint16_t height = 16) {
		ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, y, ST7735Ctx.Width, height, BLACK);
	};
	SignalQuality_t quality = {0};
	uint32_t start_time = HAL_GetTick();
	CircularQueue test_queue;
	queue_init(&test_queue);
	while (1)
	{	
		//Check the time basd on user state
		uint32_t mode_time = HAL_GetTick() - start_time;
		if( mode_time >= wakeup_time) { // 10 seconds
			if(mode_time >= wakeup_time+10000){
				current_mode = FRENZY_MODE;
			}
			else{
				current_mode = WAKEUP_MODE;
			}
		}
		
		// Test Data
		// queue_enqueue(&test_queue,0xEE);
		// for(int i=0;i<150;i++){
		// 	queue_enqueue(&test_queue, i);
		// }
		// queue_enqueue(&test_queue,0XAA);

		// Check the flags ON
		if(uart1_rx_flag){
			uart1_rx_flag = 0;
			sprintf((char *)&text, "Uart1 Rx : %c\r\n", uart1_rx_data);
			UART_Send_String((char *)text);
		}
		
		#ifdef USE_UART2
		if(uart2_rx_flag){
			uart2_rx_flag = 0;
			if(queue_is_full(&uart2_rx_queue)){
				sprintf((char *)&text, "Queue Full! Dequeueing...\r\n");
				UART_Send_String((char *)text);
				// Dequeue the oldest data if the queue is full
				queue_dequeue(&uart2_rx_queue);
			}
		}
		#endif

		// #ifdef USE_UART3
		// if(uart3_rx_flag){
		// 	uart3_rx_flag = 0;
		// 	if(test_mode == 0){
		// 		DataTransReceive_PingPongTest();
		// 	}
		// 	init_packet.start_byte = queue_dequeue(&uart3_rx_queue);
		// 	init_packet.msg_type = queue_dequeue(&uart3_rx_queue);
		// 	init_packet.data = queue_dequeue(&uart3_rx_queue);
		// 	init_packet.end_byte = queue_dequeue(&uart3_rx_queue);
		// 	sprintf((char *)text, "Uart3 Rx : %02X %02X %02X %02X\r\n", 
		// 			init_packet.start_byte, init_packet.msg_type, init_packet.data, init_packet.end_byte);
		// 	UART_Send_String((char *)text);
		// 	// Process the received packet
		// }
		// #endif

		if(current_mode == SLEEP_MODE){
			// Process the I and Q data queues
			if(IQ_ConvertQueueToIQQueues(&test_queue, &i_data_queue, &q_data_queue) != HAL_OK){
				// UART_Send_String("Error converting UART data to I/Q queues\r\n");
				continue;
			}
			if(IQ_CheckIQQueuesReady(&i_data_queue, &q_data_queue) == HAL_OK){
				// 처리된 I/Q 데이터를 저장할 버퍼
				static float i_data_buffer[250];
				static float q_data_buffer[250];
				
				if(IQ_ProcessFloatQueues(&i_data_queue, &q_data_queue, &quality, i_data_buffer, q_data_buffer) == HAL_OK){
					// sprintf((char *)text, "Phase STD: %.6f, Quality: %.2f\r\n", 
					// 		quality.phase_std, quality.final_quality);
					// UART_Send_String((char *)text);
				}
				//movement detection result - 이제 동일한 데이터 사용
				float movement_level = Movement_CalculateLevel(i_data_buffer, q_data_buffer);
				//sprintf((char*)text, "Movement Level: %.6f\r\n", movement_level);
				//UART_Send_String((char*)text);
				
				// 입력 텐서 크기 확인 후 안전하게 설정
				int input_size = input->dims->data[1];
				if (input_size >= 3) {
					input->data.f[0] = movement_level;        // Movement level
					input->data.f[1] = quality.final_quality; // Quality score
					input->data.f[2] = quality.phase_std;     // Phase std
				} else if (input_size >= 2) {
					input->data.f[0] = movement_level;        // Movement level
					input->data.f[1] = quality.final_quality; // Quality score
				} else if (input_size >= 1) {
					input->data.f[0] = movement_level;        // Movement level only
				}
				// Run the model
				uint32_t time_start_invoke = HAL_GetTick();
				if(interpreter.Invoke() != kTfLiteOk){
					sprintf((char *)&text, "TFLM Invoke Failed!!\r\n");
					UART_Send_String((char*)text);
					while(1);
				}
				uint32_t time_end_invoke = HAL_GetTick();
				sprintf((char *)&text, "invoke : %lums\n\r",(unsigned long)(time_end_invoke - time_start_invoke));
				UART_Send_String((char*)text);

				TfLiteTensor* output = interpreter.output(0);
				int top_prediction = GetSmoothedPrediction(output->data.f, &prob_queue);
				// sprintf((char *)&text, "Top Prediction: %d\r\n", top_prediction);
				// UART_Send_String((char*)text);	
				if(top_prediction == 0){
					current_user_state = UNKNOWN;
					awake_count = 0; // Reset awake count
				}
				else if(top_prediction == 1){
					current_user_state = SLEEP;
					awake_count = 0; // Reset awake count
				}
				else if(top_prediction == 2){
					if(awake_count < 2){
						awake_count++;
					}
					else{
						current_user_state = AWAKE;
					}
					current_user_state = UNKNOWN;
				}
				else{
					continue;
				}
				if(current_user_state == SLEEP){
					IoT_SendCommand(IOT_CMD_LIGHT_OFF);
					IoT_SendCommand(IOT_CMD_AC_OFF);
					IoT_SendCommand(IOT_CMD_TV_OFF);
					IoT_SendCommand(IOT_CMD_SPEAKER_OFF);
					alarm_flag = true; // Reset alarm flag
				}
				else if(current_user_state == AWAKE){
					alarm_flag = false; // Reset alarm flag
				}
				else{
					alarm_flag = true; // Reset alarm flag
				}
			}
			else{
				continue;
			}
		}
		else if(current_mode == WAKEUP_MODE){
			if (alarm_flag) {
				// (a) Alarm flag가 On일 경우
				SimplePacket_t packet;
				packet.start_byte = PACKET_START_BYTE;
				packet.msg_type = MSG_FLAG;
				packet.data = 0x01; // 알람 플래그 ON
				packet.end_byte = PACKET_END_BYTE;
				
				SendPacket(&packet);
				alarm_sent = true; // 알람 플래그 전송 완료
				// 초기화 - 알람 시작 시간 저장
				Buzzer_On();
				if(IQ_ConvertQueueToIQQueues(&test_queue, &i_data_queue, &q_data_queue) != HAL_OK){
					// UART_Send_String("Error converting UART data to I/Q queues\r\n");
					continue;
				}
				if(IQ_CheckIQQueuesReady(&i_data_queue, &q_data_queue) == HAL_OK){
					// 처리된 I/Q 데이터를 저장할 버퍼
					static float i_data_buffer2[250];
					static float q_data_buffer2[250];
					
					if(IQ_ProcessFloatQueues(&i_data_queue, &q_data_queue, &quality, i_data_buffer2, q_data_buffer2) == HAL_OK){
					}
					//movement detection result - 이제 동일한 데이터 사용
					float movement_level = Movement_CalculateLevel(i_data_buffer2, q_data_buffer2);
					
					// 입력 텐서 크기 확인 후 안전하게 설정
					int input_size = input->dims->data[1];
					if (input_size >= 3) {
						input->data.f[0] = movement_level;        // Movement level
						input->data.f[1] = quality.final_quality; // Quality score
						input->data.f[2] = quality.phase_std;     // Phase std
					} else if (input_size >= 2) {
						input->data.f[0] = movement_level;        // Movement level
						input->data.f[1] = quality.final_quality; // Quality score
					} else if (input_size >= 1) {
						input->data.f[0] = movement_level;        // Movement level only
					}
					// Run the model
					uint32_t time_start_invoke = HAL_GetTick();
					if(interpreter.Invoke() != kTfLiteOk){
						sprintf((char *)&text, "TFLM Invoke Failed!!\r\n");
						UART_Send_String((char*)text);
						while(1);
					}
					uint32_t time_end_invoke = HAL_GetTick();
					sprintf((char *)&text, "invoke : %lums\n\r",(unsigned long)(time_end_invoke - time_start_invoke));
					UART_Send_String((char*)text);

					TfLiteTensor* output = interpreter.output(0);
					int top_prediction = GetSmoothedPrediction(output->data.f, &prob_queue);
					if(top_prediction == 0){
						current_user_state = UNKNOWN;
						awake_count = 0; // Reset awake count
					}
					else if(top_prediction == 1){
						current_user_state = SLEEP;
						awake_count = 0; // Reset awake count
					}
					else if(top_prediction == 2){
						if(awake_count < 2){
							awake_count++;
						}
						else{
							current_user_state = AWAKE;
						}
						current_user_state = UNKNOWN;
					}
					else{
						continue;
					}
				}
				if(uart3_rx_flag){
					uart3_rx_flag = 0;
					SimplePacket_t received_packet;
					ReceivePacket(&received_packet);
					if(received_packet.msg_type == MSG_DATA && received_packet.data == 0x00){
						start_time = HAL_GetTick(); // 알람 시작 시간 초기화
						wakeup_time = 20000; // 20초로 설정
						current_mode = SLEEP_MODE; // 슬립 모드로 전환
						//snooze -> 마이크 off
						SimplePacket_t wakeup_packet;
						wakeup_packet.start_byte = PACKET_START_BYTE;
						wakeup_packet.msg_type = MSG_FLAG;
						wakeup_packet.data = 0x00; // 마이크 OFF
						wakeup_packet.end_byte = PACKET_END_BYTE;
						SendPacket(&wakeup_packet);

					}
					else if(received_packet.msg_type == MSG_FLAG && received_packet.data == 0x01){
						start_time = HAL_GetTick(); // 알람 시작 시간 초기화
						wakeup_time = 10000; // 10초로 설정
						current_mode = SLEEP_MODE; // 슬립 모드로 전환
						//snooze -> 마이크 off
						SimplePacket_t wakeup_packet;
						wakeup_packet.start_byte = PACKET_START_BYTE;
						wakeup_packet.msg_type = MSG_FLAG;
						wakeup_packet.data = 0x00; // 마이크 OFF
						wakeup_packet.end_byte = PACKET_END_BYTE;
						SendPacket(&wakeup_packet);
					}
				}
				if(current_user_state == AWAKE){
					alarm_flag = false; // Reset alarm flag
					Buzzer_Off();
					SimplePacket_t wakeup_packet;
					wakeup_packet.start_byte = PACKET_START_BYTE;
					wakeup_packet.msg_type = MSG_FLAG;
					wakeup_packet.data = 0x00; // 마이크 OFF
					wakeup_packet.end_byte = PACKET_END_BYTE;
					SendPacket(&wakeup_packet);
					break; // 기상 모드 종료
				}
			}
			else{
				// (b) Alarm flag가 Off일 경우
				SimplePacket_t wakeup_packet;
				wakeup_packet.start_byte = PACKET_START_BYTE;
				wakeup_packet.msg_type = MSG_FLAG;
				wakeup_packet.data = 0x00; // 마이크 OFF
				wakeup_packet.end_byte = PACKET_END_BYTE;
				SendPacket(&wakeup_packet);
				break; // 기상 모드 종료
			}
		}
		else if(current_mode == FRENZY_MODE){
			// 긴급 모드 처리 (추후 구현)
			// 기본적으로 더 강력한 알람 처리
			IoT_SendCommand(IOT_CMD_SPEAKER_ON);
			IoT_SendCommand(IOT_CMD_TV_ON);
			IoT_SendCommand(IOT_CMD_LIGHT_ON);
			IoT_SendCommand(IOT_CMD_AC_ON);

			if(IQ_ConvertQueueToIQQueues(&test_queue, &i_data_queue, &q_data_queue) != HAL_OK){
				// UART_Send_String("Error converting UART data to I/Q queues\r\n");
				continue;
			}
			if(IQ_CheckIQQueuesReady(&i_data_queue, &q_data_queue) == HAL_OK){
				// 처리된 I/Q 데이터를 저장할 버퍼
				static float i_data_buffer3[250];
				static float q_data_buffer3[250];
				
				if(IQ_ProcessFloatQueues(&i_data_queue, &q_data_queue, &quality, i_data_buffer3, q_data_buffer3) == HAL_OK){
				}
				//movement detection result - 이제 동일한 데이터 사용
				float movement_level = Movement_CalculateLevel(i_data_buffer3, q_data_buffer3);
				IoT_SendCommand(IOT_CMD_LIGHT_OFF);
				IoT_SendCommand(IOT_CMD_AC_OFF);
				// 입력 텐서 크기 확인 후 안전하게 설정
				int input_size = input->dims->data[1];
				if (input_size >= 3) {
					input->data.f[0] = movement_level;        // Movement level
					input->data.f[1] = quality.final_quality; // Quality score
					input->data.f[2] = quality.phase_std;     // Phase std
				} else if (input_size >= 2) {
					input->data.f[0] = movement_level;        // Movement level
					input->data.f[1] = quality.final_quality; // Quality score
				} else if (input_size >= 1) {
					input->data.f[0] = movement_level;        // Movement level only
				}
				// Run the model
				uint32_t time_start_invoke = HAL_GetTick();
				if(interpreter.Invoke() != kTfLiteOk){
					sprintf((char *)&text, "TFLM Invoke Failed!!\r\n");
					UART_Send_String((char*)text);
					while(1);
				}
				uint32_t time_end_invoke = HAL_GetTick();
				sprintf((char *)&text, "invoke : %lums\n\r",(unsigned long)(time_end_invoke - time_start_invoke));
				UART_Send_String((char*)text);

				TfLiteTensor* output = interpreter.output(0);
				int top_prediction = GetSmoothedPrediction(output->data.f, &prob_queue);
				if(top_prediction == 0){
					current_user_state = UNKNOWN;
					awake_count = 0; // Reset awake count
				}
				else if(top_prediction == 1){
					current_user_state = SLEEP;
					awake_count = 0; // Reset awake count
				}
				else if(top_prediction == 2){
					if(awake_count < 2){
						awake_count++;
					}
					else{
						current_user_state = AWAKE;
					}
					current_user_state = UNKNOWN;
				}
				else{
					continue;
				}
				IoT_SendCommand(IOT_CMD_LIGHT_ON);
				IoT_SendCommand(IOT_CMD_AC_ON);
			}
			if(uart3_rx_flag){
				uart3_rx_flag = 0;
				SimplePacket_t received_packet;
				ReceivePacket(&received_packet);
				if(received_packet.msg_type == MSG_DATA && received_packet.data == 0x00){
					start_time = HAL_GetTick(); // 알람 시작 시간 초기화
					wakeup_time = 20000; // 20초로 설정
					current_mode = SLEEP_MODE; // 슬립 모드로 전환
					SimplePacket_t wakeup_packet;
					wakeup_packet.start_byte = PACKET_START_BYTE;
					wakeup_packet.msg_type = MSG_FLAG;
					wakeup_packet.data = 0x00; // 마이크 OFF
					wakeup_packet.end_byte = PACKET_END_BYTE;
					SendPacket(&wakeup_packet);
				}
				else if(received_packet.msg_type == MSG_FLAG && received_packet.data == 0x01){
					start_time = HAL_GetTick(); // 알람 시작 시간 초기화
					wakeup_time = 10000; // 10초로 설정
					current_mode = SLEEP_MODE; // 슬립 모드로 전환
					SimplePacket_t wakeup_packet;
					wakeup_packet.start_byte = PACKET_START_BYTE;
					wakeup_packet.msg_type = MSG_FLAG;
					wakeup_packet.data = 0x00; // 마이크 OFF
					wakeup_packet.end_byte = PACKET_END_BYTE;
					SendPacket(&wakeup_packet);
				}
			}
				if(current_user_state == AWAKE){
					alarm_flag = false; // Reset alarm flag
					IoT_SendCommand(IOT_CMD_TV_OFF);
					IoT_SendCommand(IOT_CMD_SPEAKER_OFF);
					IoT_SendCommand(IOT_CMD_LIGHT_OFF);
					IoT_SendCommand(IOT_CMD_AC_OFF);
					Buzzer_Off();
					SimplePacket_t wakeup_packet;
					wakeup_packet.start_byte = PACKET_START_BYTE;
					wakeup_packet.msg_type = MSG_FLAG;
					wakeup_packet.data = 0x00; // 마이크 OFF
					wakeup_packet.end_byte = PACKET_END_BYTE;
					SendPacket(&wakeup_packet);
					break; // 기상 모드 종료
				}
			}
			IoT_SendCommand(IOT_CMD_LIGHT_ON);
			IoT_SendCommand(IOT_CMD_AC_ON);
		}
		
		// IoT 제어 명령 전송 (조명, 에어컨, TV, 스피커 테스트)
		// IoT_SendCommand(IOT_CMD_LIGHT_ON);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_LIGHT_OFF);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_AC_ON);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_AC_OFF);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_TV_ON);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_TV_OFF);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_SPEAKER_ON);
		// HAL_Delay(500);
		// IoT_SendCommand(IOT_CMD_SPEAKER_OFF);
		// HAL_Delay(500);
}
	/* USER CODE END 2 */

/* ===================================================================
 * 메인 로직 함수
 * ===================================================================*/

/**
 * @brief  최근 10개의 확률 배열에 가중치를 적용하여 가장 가능성 높은 상태를 반환합니다.
 * @param  new_probs:    새로운 예측 확률 배열 (크기: NUM_CLASSES)
 * @param  queue:        이전 예측 확률들을 저장하는 큐의 포인터
 * @retval 가장 확률이 높은 클래스 인덱스 (0, 1, 2) 또는 데이터 부족 시 4
 */
int GetSmoothedPrediction(const float* new_probs, ProbQueue* queue)
{
    // 1. 새로운 예측값을 큐에 추가
    PredictionProbs new_data;
    memcpy(new_data.probs, new_probs, sizeof(new_data.probs));
    ProbQueue_Enqueue(queue, &new_data);

    // 2. 큐에 데이터가 충분하지 않으면 4를 반환
    if (ProbQueue_Size(queue) < MAX_PROB_QUEUE_SIZE) {
        return 4; // 데이터 부족
    }
    
    // 3. 가중 평활화 계산
    float weighted_sums[NUM_CLASSES] = {0.0f};
    float weight_sum = 0.0f;
    
    for (int i = 0; i < MAX_PROB_QUEUE_SIZE; i++) {
        float weight = (float)(i + 1); // 가중치: 1, 2, ..., 10
        PredictionProbs current_probs;
        
        // 큐에서 i번째 데이터를 가져옴 (오래된 데이터부터 순서대로)
        if (ProbQueue_Peek(queue, i, &current_probs)) {
            // 각 클래스(0, 1, 2)의 확률에 가중치를 곱해 누적
            for (int j = 0; j < NUM_CLASSES; j++) {
                weighted_sums[j] += current_probs.probs[j] * weight;
            }
            weight_sum += weight;
        }
    }
    
    // 가중 평균 계산
    float smoothed_probs[NUM_CLASSES];
    if (weight_sum > 0) {
        for (int j = 0; j < NUM_CLASSES; j++) {
            smoothed_probs[j] = weighted_sums[j] / weight_sum;
        }
    }

    // 4. 가장 높은 확률의 인덱스(argmax)를 찾아 반환
    int max_index = 0;
    float max_prob = -1.0f;
    for (int j = 0; j < NUM_CLASSES; j++) {
        if (smoothed_probs[j] > max_prob) {
            max_prob = smoothed_probs[j];
            max_index = j;
        }
    }
    
    return max_index;
}



/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY); 	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 96;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
			|RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI48, RCC_MCODIV_4);
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while (1)
	{
		LED_Blink(5, 250);
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

// IoT 제어 함수들 구현
void IoT_SendCommand(const char* command)
{
	// 송신 (IoT 명령)
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_Delay(2);
	Thread_SPI_SendPacket(&hspi3, THREAD_SPI_CMD_SEND, (uint8_t*)command, strlen(command));
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

	UART_Send_String((char*)command);
	UART_Send_String((char*)"\r\n");

	HAL_Delay(3000);
}
