/**
  ******************************************************************************
  * @file    uart.c
  * @brief   This file provides code for the configuration
  *          of uart for debug.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "queue.h"

#ifdef USE_UART2
extern CircularQueue uart2_rx_queue;
#endif

#ifdef USE_UART3
extern CircularQueue uart3_rx_queue;
#endif

volatile int uart1_rx_flag;
volatile char uart1_rx_data;

#ifdef USE_UART2
volatile int uart2_rx_flag;
volatile char uart2_rx_data;
#endif
#ifdef USE_UART3
volatile int uart3_rx_flag;
volatile char uart3_rx_data;
#endif

UART_HandleTypeDef huart1;
#ifdef USE_UART2
UART_HandleTypeDef huart2;
#endif
#ifdef USE_UART3
UART_HandleTypeDef huart3;
#endif

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void UART_Init(void){
	UART1_Init();
#ifdef USE_UART2
	UART2_Init();
#endif
#ifdef USE_UART3
	UART3_Init();
#endif
}

// 각 USART/UART 장치별로 작성 -- 장치가 추가되면 장치별로 이름 바꿔가면서 추가
void UART1_Init(void){	
  huart1.Instance        = USART1;
  huart1.Init.BaudRate   = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits   = UART_STOPBITS_1;
  huart1.Init.Parity     = UART_PARITY_NONE;
  huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart1.Init.Mode       = UART_MODE_TX_RX;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&huart1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  huart1.RxISR = USART1_Rx_ISR;
}

#ifdef USE_UART2
void UART2_Init(void){
  huart2.Instance        = USART2;
  huart2.Init.BaudRate   = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits   = UART_STOPBITS_1;
  huart2.Init.Parity     = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart2.Init.Mode       = UART_MODE_TX_RX;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&huart2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();    
  }

  huart2.RxISR = USART2_Rx_ISR;
}
#endif

#ifdef USE_UART3
void UART3_Init(void){
  huart3.Instance        = USART3;
  huart3.Init.BaudRate   = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits   = UART_STOPBITS_1;
  huart3.Init.Parity     = UART_PARITY_NONE;
  huart3.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart3.Init.Mode       = UART_MODE_TX_RX;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&huart3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();    
  }

  huart3.RxISR = USART3_Rx_ISR;
}
#endif


// 모든 USART/UART 장치 통합 작성 -- 이 코드는 HAL_UART_Init이 실행될 때 해당 함수 실행. 그래서 전체 장치에 대한 코드가 같이 있어야 해.
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	if(huart->Instance == USART1){
		/* Enable USART1 clock */
		__HAL_RCC_USART1_CLK_ENABLE();
		
		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
		RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

		HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
#ifdef USE_UART2
  else if(huart->Instance == USART2){
		/* Enable USART2 clock */
		__HAL_RCC_USART2_CLK_ENABLE();
		
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
		RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

		HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
#endif
#ifdef USE_UART3
  else if(huart->Instance == USART3){
		/* Enable USART3 clock */
		__HAL_RCC_USART3_CLK_ENABLE();
		
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
		RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

		HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
	}
#endif
}

void UART_Send_Char(char ch){
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY); 
}

void UART_Send_String(char *str){
	HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY); 
}

#ifdef USE_UART2
// UART2 transmission functions for mmWave Radar communication
void UART2_Send_Data(uint8_t* data, uint8_t length) {
	HAL_UART_Transmit(&huart2, data, length, HAL_MAX_DELAY);
}

// UART2 transmission function with status return for error handling
HAL_StatusTypeDef UART2_Send_Data_WithStatus(uint8_t* data, uint8_t length) {
	return HAL_UART_Transmit(&huart2, data, length, HAL_MAX_DELAY);
}
#endif

#ifdef USE_UART3
// UART3 transmission functions for board-to-board communication
void UART3_Send_Data(uint8_t* data, uint8_t length) {
	HAL_UART_Transmit(&huart3, data, length, HAL_MAX_DELAY);
}

// UART3 transmission function with status return for error handling
HAL_StatusTypeDef UART3_Send_Data_WithStatus(uint8_t* data, uint8_t length) {
	return HAL_UART_Transmit(&huart3, data, length, HAL_MAX_DELAY);
}
#endif

void USART1_Rx_ISR(UART_HandleTypeDef* huart){
	uart1_rx_flag = 1;
	HAL_UART_Receive(huart, (uint8_t*)&uart1_rx_data, 1, HAL_MAX_DELAY);
}

#ifdef USE_UART2
void USART2_Rx_ISR(UART_HandleTypeDef* huart){
	uart2_rx_flag = 1;
	HAL_UART_Receive(huart, (uint8_t*)&uart2_rx_data, 1, HAL_MAX_DELAY);
  queue_enqueue(&uart2_rx_queue, uart2_rx_data);
}
#endif
#ifdef USE_UART3
void USART3_Rx_ISR(UART_HandleTypeDef* huart){
	uart3_rx_flag = 1;
	HAL_UART_Receive(huart, (uint8_t*)&uart3_rx_data, 1, HAL_MAX_DELAY);
	queue_enqueue(&uart3_rx_queue, uart3_rx_data);
}
#endif
