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
#include "queue.h"
#include "buzzer.h"
#include "sleepbreathing.h"
#include "data_trans_receive.h"

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

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CircularQueue uart2_rx_queue; // UART Data Buffer
bool alarm_flag = true; // alarm flag
uint32_t wakeup_time = 1800000; // 
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

	#ifdef USE_UART3
	// Initialize Data Transmission/Reception module
	DataTransReceive_Init(BOARD_A);  // Change to BOARD_B for slave board
	
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

	while (1)
	{
		LCD_ShowString(0, 0, ST7735Ctx.Width, 16, 16, (uint8_t*)"Running...");
		// 테스트용 데이터 생성 (카운터 포함)
		#ifdef USE_UART3	
		// Run different test modes
		if (test_mode == 0) {
			// Ping-Pong Test Mode
			DataTransReceive_PingPongTest();
		} else {
			// Flag-based Data Transmission Mode
			SimplePacket_t send_packet = {0};
			send_packet.start_byte = 0xAA; // 시작 바이트
			send_packet.msg_type = MSG_FLAG; // 메시지 타입
			send_packet.data = 0x01; // Flag 데이터
			send_packet.end_byte = 0x55; // 종료 바이트
			if(SendPacket(&send_packet) != HAL_OK){
				LCD_ShowString(0, 0, ST7735Ctx.Width, 16, 16, (uint8_t*)"Sending Packet error");
				for(;;); // system halt with error
			} // Send the packet
			HAL_Delay(100); // Delay to simulate processing time
			SimplePacket_t receive_packet = {0};
			if(ReceivePacket(&receive_packet) == HAL_OK){
				// Process the received packet
				if(receive_packet.start_byte == 0xAA && receive_packet.end_byte == 0x55){
					sprintf((char *)text, "Received Flag: %02X\r\n", receive_packet.data);
					UART_Send_String((char *)text);
				} else {
					UART_Send_String("Invalid Packet Received!\r\n");
				}
			} else {
				UART_Send_String("No Packet Received!\r\n");
			}
		}	
		#endif
		
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

		if(uart1_rx_flag){
			uart1_rx_flag = 0;
			sprintf((char *)&text, "Uart1 Rx : %c\r\n", uart1_rx_data);
			UART_Send_String((char *)text);
		}
		HAL_Delay(100);

		// Send the data to mmWave Radar via UART2
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
		radar_data_process(&uart2_rx_queue);
		#endif

		HAL_Delay(100);
		// Enhanced Buzzer Test - Test every 10 iterations
		#ifdef USE_BUZZER
		if (test_counter % 10 == 0) {
			UART_Send_String("=== Buzzer Test Started! ===\r\n");
			HAL_Delay(100);
			
			// Test sequence: Short beep, pause, longer beep, pause, triple beep
			Buzzer_Test(500);   // 500ms beep
			HAL_Delay(300);     // 300ms pause
			
			// Triple beep
			for(int i = 0; i < 3; i++) {
				Buzzer_Test(130);   // 130ms beep
				HAL_Delay(80);     // 80ms pause between beeps
			}
		}
		#endif

		test_counter++;
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
