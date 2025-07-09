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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "uart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"

#include <stdio.h>

#include "thread_spi.h"

#include "queue.h"
#include "buzzer.h"
#include "sleepbreathing.h"
#include "data_trans_receive.h"

#include "i2s.h"
#include "arm_math.h"
#include "tensorflow/lite/core/c/common.h"

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/recording_micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/micro_profiler.h"

#include "models/kws_pqt_int_model_mfcc13_nmels40_silence_unknown_DSCNNSmall.h"


// === Config & Globals ===
#define AUDIO_BUF_LEN 1024
#define FRAME_SIZE 512
#define HOP_SIZE   256
#define FFT_SIZE 512
#define NUM_MEL 40
#define NUM_MFCC 13
#define SAMPLE_RATE 16000
#define NUM_FRAMES (((SAMPLE_RATE - FRAME_SIZE) / HOP_SIZE) + 1)
#define M_PI 3.14159265358979323846f

static int32_t audioBuf[AUDIO_BUF_LEN];
extern const int16_t audio_data[16000];
static int16_t audio_circbuf[2048];
static int circbuf_idx = 0;
#define MODEL_INPUT_BUFFER_SIZE (NUM_FRAMES * 10)
static float32_t model_input[MODEL_INPUT_BUFFER_SIZE][NUM_MFCC];
static int model_inputbuf_idx = 0;
static uint32_t model_input_start_idx = 0;
float32_t hann_window[FRAME_SIZE];
float32_t mel_filterbank[NUM_MEL][FFT_SIZE/2+1];
float32_t input_frame[FRAME_SIZE];
float32_t fft_out[FFT_SIZE];
float32_t mel_energies[NUM_MEL];
float32_t mfcc[NUM_MFCC];
arm_dct4_instance_f32 dct4;
arm_rfft_instance_f32 S_RFFT;
arm_cfft_radix4_instance_f32 S_CFFT;
arm_rfft_fast_instance_f32 S;
float32_t dct4_state[NUM_MEL];
float32_t dct4_buffer[NUM_MEL];

static int frame_counter = 0;
static volatile int model_input_ready = 0;
static int stride_counter = -1;

static uint32_t processing_start_time = 0;
static uint32_t processing_end_time = 0;
constexpr int tensor_arena_size = 100 * 1024;
static uint8_t tensor_arena[tensor_arena_size];
static int result;

// === 함수 선언 ===
void SystemClock_Config(void);
void compute_mfcc(const int16_t *pcm, float32_t *mfcc_out);
void create_mel_filterbank(float32_t mel_filterbank[NUM_MEL][FFT_SIZE/2+1], int sample_rate, int fft_size, int num_mel, int htk);
void dct2_ortho(const float *in, float *out, int N, int K);
void process_mfcc_if_ready();
void push_samples_to_circbuf(const int32_t *src, int n);
void compute_mean_std(const float32_t model_input[MODEL_INPUT_BUFFER_SIZE][NUM_MFCC], float32_t *mean, float32_t *std, uint32_t model_input_start_idx);
int run_model_inference(tflite::MicroInterpreter* interpreter, const float32_t model_input[MODEL_INPUT_BUFFER_SIZE][NUM_MFCC], uint32_t model_input_start_idx);
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// UART Data Buffer
CircularQueue uart1_rx_queue;
CircularQueue uart2_rx_queue;

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

// Ping-Pong 테스트 루프 함수 분리
void PingPongTestLoop(void);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	MPU_Config();
	CPU_CACHE_Enable();
	
	// DWT 사이클 카운터 초기화
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	DWT->CYCCNT = 0;
	
	/* USER CODE END 1 */
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI4_Init();
	MX_SPI3_Init();
	MX_I2S1_Init();

	// 이걸 왜 하는 지 모르겠어.
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	MX_TIM1_Init();
	UART_Init();

	HAL_TIM_Base_Start(&htim1);
	arm_rfft_fast_init_f32(&S, FFT_SIZE);
	/* USER CODE BEGIN 2 */
	LCD_Test();
	uint8_t text[32];

	// Initialize UART RX Queues
	//queue_init(&uart1_rx_queue);
	queue_init(&uart2_rx_queue);

	#ifdef USE_UART3
	// Initialize Data Transmission/Reception module
//	DataTransReceive_Init(BOARD_A);  // Change to BOARD_B for slave board
	DataTransReceive_Init(BOARD_B);  // Change to BOARD_B for slave board
	
	// Test mode selection
	uint8_t test_mode = 0; // 0: Ping-Pong test, 1: Flag-based data transmission
	uint32_t mode_switch_time = HAL_GetTick();
	#endif

	uint32_t test_counter = 0;
	Thread_SPI_Packet_t received_packet;

	UART_Send_String("Thread IoT SPI Bidirectional Test Started!\r\n");

	ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width, ST7735Ctx.Height, BLACK);

	auto clearLine = [](uint16_t y, uint16_t height = 16) {
		ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, y, ST7735Ctx.Width, height, BLACK);
	};

	// PingPongTestLoop(); // 필요할 때만 호출
	#ifdef USE_UART2
	uint8_t data[7] = {0x55, 0x06, 0x00, 0x02, 0x05, 0x0D, 0x01};
	makechecksum(data, 7); // Create checksum for the data
	#endif

	// === DSP/ML 관련 초기화 ===
	for (int i = 0; i < FRAME_SIZE; i++) {
		hann_window[i] = 0.5f * (1.0f - cosf(2.0f * M_PI * i / (FRAME_SIZE)));
	}
	create_mel_filterbank(mel_filterbank, SAMPLE_RATE, FFT_SIZE, NUM_MEL, 0);

	// === 모델 초기화 ===
	const tflite::Model* model = ::tflite::GetModel(kws_pqt_int_model_mfcc13_nmels40_silence_unknown_DSCNNSmall);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		for(;;);
	}
	tflite::MicroMutableOpResolver<6> micro_op_resolver;
	micro_op_resolver.AddConv2D();
	micro_op_resolver.AddDepthwiseConv2D();
	micro_op_resolver.AddMean();
	micro_op_resolver.AddFullyConnected();
//	micro_op_resolver.AddTranspose();
//	micro_op_resolver.AddMaxPool2D();
//	micro_op_resolver.AddReshape();
	micro_op_resolver.AddSoftmax();
//	uint8_t text[80];
	tflite::MicroInterpreter interpreter(model, micro_op_resolver, tensor_arena, tensor_arena_size);
	if(interpreter.AllocateTensors() != kTfLiteOk){
		while(1);
	}
	TfLiteTensor* input = interpreter.input(0);
	// sprintf((char *)&text, "Tensor Arena used : %d\n\r", interpreter.arena_used_bytes());
	// UART_Send_String((char*)text);
	// sprintf((char *)&text, "Input_Shape : (%d,%d,%d)\n\r",input->dims->data[0], input->dims->data[1], input->dims->data[2]) ;
	// UART_Send_String((char*)text);
	if (HAL_I2S_Receive_DMA(&hi2s1, (uint16_t*)audioBuf, AUDIO_BUF_LEN) != HAL_OK) {
//		__HAL_DMA_ENABLE_IT(&hdma_spi1_rx, DMA_IT_TC | DMA_IT_HT);
		Error_Handler();
	}
	bool alarm_flag = Receive_Flag();
	SimplePacket_t packet;

	while (1)
	{
//	  if (model_input_ready) {
//		model_input_start_idx = (model_inputbuf_idx - NUM_FRAMES + MODEL_INPUT_BUFFER_SIZE) % MODEL_INPUT_BUFFER_SIZE - 1;
//		processing_start_time = HAL_GetTick();
//		result = run_model_inference(&interpreter, model_input, model_input_start_idx);
//		processing_end_time = HAL_GetTick();
//
//		// Processing time 계산 및 UART 출력
//		uint32_t processing_time = processing_end_time - processing_start_time;
//		char timing_str[128];
//		sprintf(timing_str, "Model Inference Time: %lu ms\r\n", processing_time);
//		UART_Send_String(timing_str);
//
//		model_input_ready = 0;
//	  }
		LCD_ShowString(0, 0, ST7735Ctx.Width, 16, 16, (uint8_t*)"Running...");
		// 테스트용 데이터 생성 (카운터 포함)
		#ifdef USE_UART3
		if(uart3_rx_flag){
			uart3_rx_flag = 0;
			if(queue_is_full(&uart3_rx_queue)){
				sprintf((char *)&text, "Queue Full! Dequeueing...\r\n");
				UART_Send_String((char *)text);
				// Dequeue the oldest data if the queue is full
				queue_dequeue(&uart3_rx_queue);
			}
		}
		#endif
		#ifdef USE_UART3
		if (test_mode == 0) {
			// Ping-Pong Test Mode
			DataTransReceive_PingPongTest();
		} else {
			// Flag-based Data Transmission Mode
			if (alarm_flag){
			  if (model_input_ready) {
				model_input_start_idx = (model_inputbuf_idx - NUM_FRAMES + MODEL_INPUT_BUFFER_SIZE) % MODEL_INPUT_BUFFER_SIZE - 1;
				processing_start_time = HAL_GetTick();
				result = run_model_inference(&interpreter, model_input, model_input_start_idx);
				processing_end_time = HAL_GetTick();
				model_input_ready = 0;
				// Send Packet
				if (result == 0 || result == 1){
					packet.data = result;
					packet.start_byte = PACKET_START_BYTE;
					packet.end_byte = PACKET_END_BYTE;
					packet.msg_type = MSG_DATA;
					SendPacket(&packet);
					alarm_flag = false;
				}
			  }
			}
			else{
			  HAL_Delay(100);
			  alarm_flag = Receive_Flag();
			}
		}
//
//			// Process accumulated data
//			DataTransReceive_ProcessData();
//		}
		#endif
//
//		// 기존 Thread SPI 테스트 코드 (간소화)
//		char test_data[32];
//		snprintf(test_data, sizeof(test_data), "Hello Thread! Cnt=%lu", test_counter++);
//
//		// 송신 (Hello Thread!)
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
//		HAL_Delay(2);
//		Thread_SPI_SendPacket(&hspi3, THREAD_SPI_CMD_SEND, (uint8_t*)test_data, strlen(test_data));
//		HAL_Delay(2);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
//
//		UART_Send_String("Sent to ESP: ");
//		UART_Send_String(test_data);
//		UART_Send_String("\r\n");
//
//		// 수신: 한 번만 ReceivePacket
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
//		HAL_Delay(2);
//		Thread_SPI_Packet_t rx_packet;
//		if (Thread_SPI_ReceivePacket(&hspi3, &rx_packet) == HAL_OK) {
//			// 필요시 OK 등 수신 데이터 출력 가능
//		}
//		HAL_Delay(2);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
//
//		if(uart1_rx_flag){
//			uart1_rx_flag = 0;
//			sprintf((char *)&text, "Uart1 Rx : %c\r\n", uart1_rx_data);
//			UART_Send_String((char *)text);
//		}
//		HAL_Delay(100);
//
//		// Send the data to mmWave Radar via UART2
//		#ifdef USE_UART2
//		if(uart2_rx_flag){
//			uart2_rx_flag = 0;
//			if(queue_is_full(&uart2_rx_queue)){
//				sprintf((char *)&text, "Queue Full! Dequeueing...\r\n");
//				UART_Send_String((char *)text);
//				// Dequeue the oldest data if the queue is full
//				queue_dequeue(&uart2_rx_queue);
//			}
//		}
//		radar_data_process(&uart2_rx_queue);
//		#endif
//
//		HAL_Delay(100);
//		// Enhanced Buzzer Test - Test every 10 iterations
//		#ifdef USE_BUZZER
//		if (test_counter % 10 == 0) {
//			UART_Send_String("=== Buzzer Test Started! ===\r\n");
//			HAL_Delay(100);
//
//			// Test sequence: Short beep, pause, longer beep, pause, triple beep
//			Buzzer_Test(500);   // 500ms beep
//			HAL_Delay(300);     // 300ms pause
//
//			// Triple beep
//			for(int i = 0; i < 3; i++) {
//				Buzzer_Test(130);   // 130ms beep
//				HAL_Delay(80);     // 80ms pause between beeps
//			}
//		}
//		#endif
	}
	/* USER CODE END 2 */
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
// DMA 콜백에서 호출: 새 샘플 push (짝수 인덱스만, 24bit->16bit 변환)
void push_samples_to_circbuf(const int32_t *src, int n) {
    for (int i = 0; i < n; i += 2) { // 짝수 인덱스만 사용
        int32_t sample24 = src[i] & 0xFFFFFF;
        if (sample24 & 0x800000) sample24 |= 0xFF000000;
        int16_t sample16 = (int16_t)(sample24 >> 8);
        audio_circbuf[circbuf_idx] = sample16;
        circbuf_idx = (circbuf_idx + 1) % 2048;  // 새로운 버퍼 크기 사용
    }
}


// === MFCC 프레임 처리 및 모델 입력 준비 ===
/**
 * @brief HOP_SIZE만큼 샘플이 쌓였을 때마다 MFCC를 추출하고 모델 입력 버퍼에 shift-in
 *        model_input_ready 플래그를 통해 모델 추론 준비 상태를 알림
 */
void process_mfcc_if_ready() {
    frame_counter++;
    if (frame_counter >= 2) {
        int start = (frame_counter == 2) ? 0 : ((frame_counter - 2) * HOP_SIZE) % 2048;
        // 1. 원형 버퍼에서 프레임 추출
        int16_t mfcc_input[FRAME_SIZE];
        for (int i = 0; i < FRAME_SIZE; i++) {
            int idx = (start + i) % 2048;
            mfcc_input[i] = audio_circbuf[idx];
        }

        // 2. MFCC 추출
        float32_t mfcc_result[NUM_MFCC];
        compute_mfcc(mfcc_input, mfcc_result);

        // 3. 모델 입력 버퍼에 shift-in
        for (int i = 0; i < NUM_MFCC; i++) {
            model_input[model_inputbuf_idx][i] = mfcc_result[i];
        }
        model_inputbuf_idx++;
        if (model_inputbuf_idx >= MODEL_INPUT_BUFFER_SIZE) {
            model_inputbuf_idx = 0;
        }

        // 4. 모델 입력이 충분히 쌓였는지 체크
        if (frame_counter > NUM_FRAMES) {
            model_input_ready = 1;
            stride_counter++;
            // (필요시) 입력 오디오 샘플 백업 등 추가 처리 가능
        }
    }
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
    uint32_t start_cycles = DWT->CYCCNT;
    // 버퍼에 데이터 쌓기 및 MFCC 추출까지 바로 처리
    push_samples_to_circbuf(&audioBuf[0], AUDIO_BUF_LEN/2);
    process_mfcc_if_ready();
    uint32_t end_cycles = DWT->CYCCNT;
    uint32_t cycles = end_cycles - start_cycles;
    // 400MHz 기준(us = cycles / 400)
//    char timing_str[64];
//    sprintf(timing_str, "DMA half: %lu cycles (~%lu us)\r\n", cycles, cycles / 400);
//    UART_Send_String(timing_str);

}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s) {
    uint32_t start_cycles = DWT->CYCCNT;
    // 버퍼에 데이터 쌓기 및 MFCC 추출까지 바로 처리
    push_samples_to_circbuf(&audioBuf[AUDIO_BUF_LEN/2], AUDIO_BUF_LEN/2);
     process_mfcc_if_ready();
    uint32_t end_cycles = DWT->CYCCNT;
    uint32_t cycles = end_cycles - start_cycles;
    // 400MHz 기준(us = cycles / 400)
//    char timing_str[64];
//    sprintf(timing_str, "DMA complete: %lu cycles (~%lu us)\r\n", cycles, cycles / 400);
//    UART_Send_String(timing_str);
}

void compute_mfcc(const int16_t *pcm, float32_t *mfcc_out) {
    // 1. Convert PCM to float and apply window
    for (int i = 0; i < FRAME_SIZE; i++){
        float32_t normalized_pcm = (float32_t)pcm[i] / 32768.0f;
        input_frame[i] = normalized_pcm * hann_window[i];
    }
    // 2. FFT
    arm_rfft_fast_f32(&S, input_frame, fft_out, 0);
    // 3. Power spectrum
    // ARM RFFT 출력: [real[0], imag[0], real[1], imag[1], real[2], imag[2], ...]
    // DC 성분 (i=0): real[0]^2 (imag[0]은 항상 0)
    float32_t power_spectrum[FFT_SIZE/2+1];
    power_spectrum[0] = fft_out[0] * fft_out[0]; // DC
    power_spectrum[FFT_SIZE/2] = fft_out[1] * fft_out[1]; // Nyquist
    for (int i = 1; i < FFT_SIZE/2; i++) {
        float32_t real_part = fft_out[2*i];
        float32_t imag_part = fft_out[2*i+1];
        power_spectrum[i] = real_part * real_part + imag_part * imag_part;
    }
    // 4. Mel filterbank
    for (int m = 0; m < NUM_MEL; m++) {
        mel_energies[m] = 0;
        for (int k = 0; k < FFT_SIZE/2+1; k++){
            mel_energies[m] += mel_filterbank[m][k] * power_spectrum[k];
        }
        // 수치적 안정성을 위해 더 큰 epsilon 사용
        mel_energies[m] = 10.0f * log10f(mel_energies[m] + 1e-10f);
    }
    // 5. DCT (MFCC)
    memcpy(dct4_buffer, mel_energies, sizeof(float32_t)*NUM_MEL);
    dct2_ortho(mel_energies, dct4_buffer, NUM_MEL, NUM_MFCC);
    memcpy(mfcc_out, dct4_buffer, sizeof(float32_t)*NUM_MFCC);
}

// Hz -> Mel 변환 (Slaney/HTK 선택)
static float hz_to_mel(float hz, int htk) {
    if (htk) {
        // HTK 방식
        if (hz <= 0.0f) return 0.0f;
        return 2595.0f * log10f(1.0f + hz / 700.0f);
    } else {
        // Slaney/librosa 방식
        float f_min = 0.0f;
        float f_sp = 200.0f / 3.0f;
        float min_log_hz = 1000.0f;
        float min_log_mel = (min_log_hz - f_min) / f_sp;
        float logstep = logf(6.4f) / 27.0f;
        float mel;
        if (hz < min_log_hz) {
            mel = (hz - f_min) / f_sp;
        } else {
            mel = min_log_mel + logf(hz / min_log_hz) / logstep;
        }
        return mel;
    }
}

// Mel -> Hz 변환 (Slaney/HTK 선택)
static float mel_to_hz(float mel, int htk) {
    if (htk) {
        // HTK 방식
        if (mel <= 0.0f) return 0.0f;
        return 700.0f * (powf(10.0f, mel / 2595.0f) - 1.0f);
    } else {
        // Slaney/librosa 방식
        float f_min = 0.0f;
        float f_sp = 200.0f / 3.0f;
        float min_log_hz = 1000.0f;
        float min_log_mel = (min_log_hz - f_min) / f_sp;
        float logstep = logf(6.4f) / 27.0f;
        float hz;
        if (mel < min_log_mel) {
            hz = f_min + f_sp * mel;
        } else {
            hz = min_log_hz * expf(logstep * (mel - min_log_mel));
        }
        return hz;
    }
}

// Mel filterbank 생성 함수 (htk 플래그 추가)
void create_mel_filterbank(
    float32_t mel_filterbank[NUM_MEL][FFT_SIZE/2+1],
    int sample_rate, int fft_size, int num_mel,
    int htk // 0: slaney, 1: htk
) {
    float f_min = 0.0f;
    float f_max = (float)sample_rate / 2.0f;
    float mel_min = hz_to_mel(f_min, htk);
    float mel_max = hz_to_mel(f_max, htk);

    // 1. FFT bin 중심 주파수 (실수형)
    float fftfreqs[FFT_SIZE/2+1];
    for (int k = 0; k < FFT_SIZE/2+1; k++) {
        fftfreqs[k] = (float)sample_rate * k / fft_size;
    }

    // 2. mel band 경계 (실수형)
    float mel_points[NUM_MEL + 2];
    for (int i = 0; i < num_mel + 2; i++) {
        mel_points[i] = mel_min + (mel_max - mel_min) * i / (num_mel + 1);
    }
    float hz_points[NUM_MEL + 2];
    for (int i = 0; i < num_mel + 2; i++) {
        hz_points[i] = mel_to_hz(mel_points[i], htk);
    }

    // 3. 삼각형 필터 계산 (실수형)
    for (int m = 0; m < num_mel; m++) {
        float f_left = hz_points[m];
        float f_center = hz_points[m+1];
        float f_right = hz_points[m+2];
        for (int k = 0; k < FFT_SIZE/2+1; k++) {
            float freq = fftfreqs[k];
            float lower = (freq - f_left) / (f_center - f_left);
            float upper = (f_right - freq) / (f_right - f_center);
            float val = fmaxf(0.0f, fminf(lower, upper));
            mel_filterbank[m][k] = val;
        }
    }

    // 4. Slaney-style 정규화 (각 필터의 에너지가 일정하도록)
    for (int m = 0; m < num_mel; m++) {
        float enorm = 2.0f / (hz_points[m+2] - hz_points[m]);
        for (int k = 0; k < FFT_SIZE/2+1; k++) {
            mel_filterbank[m][k] *= enorm;
        }
    }
}

void dct2_ortho(const float *in, float *out, int N, int K) {
    // in: N개 (log-mel), out: K개 (MFCC)
    // scipy.fftpack.dct(x, type=2, norm='ortho')와 정확히 일치하는 구현
    double factor = M_PI / (2.0 * N);
    double scale0 = sqrt(1.0 / N);
    double scale = sqrt(2.0 / N);

    for (int k = 0; k < K; ++k) {
        double sum = 0.0;
        for (int n = 0; n < N; ++n) {
            sum += (double)in[n] * cos(factor * (2 * n + 1) * k);
        }
        // k=0에는 scale0, 그 외 k>0에는 scale 적용
        out[k] = (float)((k == 0) ? scale0 * sum : scale * sum);
    }
}

// 모델 입력의 mean과 std를 계산하는 함수
void compute_mean_std(const float32_t model_input[MODEL_INPUT_BUFFER_SIZE][NUM_MFCC], float32_t *mean, float32_t *std, uint32_t model_input_start_idx) {
    // 전체 프레임에 대한 평균 계산
    for (int mfcc_idx = 0; mfcc_idx < NUM_MFCC; mfcc_idx++) {
        float32_t sum = 0.0f;
        for (int frame_idx = 0; frame_idx < NUM_FRAMES; frame_idx++) {
            int idx = (frame_idx + model_input_start_idx) % MODEL_INPUT_BUFFER_SIZE;
            sum += model_input[idx][mfcc_idx];
        }
        mean[mfcc_idx] = sum / NUM_FRAMES;
    }

    // 전체 프레임에 대한 표준편차 계산
    for (int mfcc_idx = 0; mfcc_idx < NUM_MFCC; mfcc_idx++) {
        float32_t variance_sum = 0.0f;
        for (int frame_idx = 0; frame_idx < NUM_FRAMES; frame_idx++) {
            int idx = (frame_idx + model_input_start_idx) % MODEL_INPUT_BUFFER_SIZE;
            float32_t diff = model_input[idx][mfcc_idx] - mean[mfcc_idx];
            variance_sum += diff * diff;
        }
        float32_t variance = variance_sum / NUM_FRAMES;
        std[mfcc_idx] = sqrtf(variance + 1e-8f);  // 수치적 안정성을 위한 작은 값 추가
    }
}

// 추론을 실행하는 함수 (int8 양자화 모델용)
int run_model_inference(tflite::MicroInterpreter* interpreter, const float32_t model_input[MODEL_INPUT_BUFFER_SIZE][NUM_MFCC], uint32_t model_input_start_idx) {
    // 입력 텐서 가져오기
    TfLiteTensor* input = interpreter->input(0);

    // int8 양자화 모델이므로 int8_t 포인터로 캐스팅
    int8_t* input_data = input->data.int8;
    float scale = input->params.scale;
    int zero_point = input->params.zero_point;

    float32_t mean[NUM_MFCC];
    float32_t std[NUM_MFCC];

    // 1. mean/std 계산 (최근 NUM_FRAMES 구간)
    compute_mean_std(model_input, mean, std, model_input_start_idx);

    // 2. 정규화된 입력을 별도 배열에 저장
    float32_t norm_input[NUM_FRAMES][NUM_MFCC];
    for (int frame_idx = 0; frame_idx < NUM_FRAMES; frame_idx++) {
        for (int mfcc_idx = 0; mfcc_idx < NUM_MFCC; mfcc_idx++) {
            int idx = (frame_idx + model_input_start_idx) % MODEL_INPUT_BUFFER_SIZE;
            if (std[mfcc_idx] > 1e-8f) {
                norm_input[frame_idx][mfcc_idx] = (model_input[idx][mfcc_idx] - mean[mfcc_idx]) / std[mfcc_idx];
            } else {
                norm_input[frame_idx][mfcc_idx] = 0.0f;
            }
        }
    }

    // 3. 정규화된 입력을 int8로 양자화하여 텐서에 복사
    for (int mfcc_idx = 0; mfcc_idx < NUM_MFCC; mfcc_idx++) {
        for (int frame_idx = 0; frame_idx < NUM_FRAMES; frame_idx++) {
            float float_val = norm_input[frame_idx][mfcc_idx];
            int quantized_val = (int)(float_val / scale) + zero_point;
            if (quantized_val > 127) quantized_val = 127;
            if (quantized_val < -128) quantized_val = -128;
            input_data[mfcc_idx * NUM_FRAMES + frame_idx] = (int8_t)quantized_val;
        }
    }

    // 4. 추론 실행
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        UART_Send_String("Error: Model inference failed!\r\n");
        return -1;
    }

    // 5. 결과 해석 및 출력
    TfLiteTensor* output = interpreter->output(0);
    int8_t* output_data = output->data.int8;
    float output_scale = output->params.scale;
    int output_zero_point = output->params.zero_point;
    int max_idx = 0;
    float32_t max_prob = (output_data[0] - output_zero_point) * output_scale;
    for (int i = 1; i < output->dims->data[1]; i++) {
        float32_t prob = (output_data[i] - output_zero_point) * output_scale;
        if (prob > max_prob) {
            max_prob = prob;
            max_idx = i;
        }
    }
    char result_str[256];
    sprintf(result_str, "Prediction: Class %d, Probability: %d\r\n", max_idx, (int)(max_prob * 100));
    UART_Send_String(result_str);

    return max_idx;
}


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

void PingPongTestLoop(void)
{
	while (1)
	{
		Thread_SPI_UpdateUptime();

		// Ping
		LCD_ShowString(0, 0, ST7735Ctx.Width, 16, 16, (uint8_t*)"Ping Testing...");
		UART_Send_String("--- Starting SPI transaction ---\r\n");
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
		UART_Send_String("Sending PING data...\r\n");
		HAL_StatusTypeDef ping3_result = Thread_SPI_SendPing(&hspi3);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		UART_Send_String("--- SPI transaction completed ---\r\n");

		if (ping3_result == HAL_OK) {
			UART_Send_String("SPI3: Ping sent, check ESP32C6 for Pong!\r\n");
		} else {
			UART_Send_String("SPI3: Ping failed!\r\n");
		}
		HAL_Delay(100);

		// Pong
		LCD_ShowString(0, 0, ST7735Ctx.Width, 16, 16, (uint8_t*)"Pong Testing...");
		Thread_SPI_Packet_t rx_packet;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
		HAL_StatusTypeDef pong_result = Thread_SPI_ReceivePacket(&hspi3, &rx_packet);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

		if (pong_result == HAL_OK && rx_packet.header == 0xA5 && rx_packet.command == 0x04 && rx_packet.length == 0) {
			char buf[64];
			snprintf(buf, sizeof(buf), "SPI3: PONG received! 0x%02X 0x%02X 0x%02X\r\n", rx_packet.header, rx_packet.command, rx_packet.length);
			UART_Send_String(buf);
		} else {
			UART_Send_String("SPI3: Pong receive failed or invalid packet!\r\n");
		}
		HAL_Delay(2000);
	}
}
