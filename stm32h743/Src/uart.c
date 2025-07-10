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

/* DMA Buffer for UART2 Reception */
#ifdef USE_UART2
static uint8_t uart2_dma_buffer[UART2_DMA_BUFFER_SIZE] __attribute__((aligned(4)));
static volatile uint16_t uart2_dma_data_size = 0;
static volatile uint8_t uart2_dma_data_ready = 0;
DMA_HandleTypeDef hdma_uart2_rx;  // Remove static to make it accessible from stm32h7xx_it.c
#endif

volatile int uart1_rx_flag;
volatile char uart1_rx_data;

#ifdef USE_UART2
volatile int uart2_rx_flag;
// volatile char uart2_rx_data;
volatile char uart2_rx_data[152];
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
  __HAL_RCC_USART1_CLK_ENABLE();
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
  __HAL_RCC_USART2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();
  
  huart2.Instance        = USART2;
  huart2.Init.BaudRate   = 115200;
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

  // DMA configuration for UART2 RX
  hdma_uart2_rx.Instance = DMA1_Stream5;
  hdma_uart2_rx.Init.Request = DMA_REQUEST_USART2_RX;
  hdma_uart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_uart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_uart2_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_uart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_uart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_uart2_rx.Init.Mode = DMA_CIRCULAR;
  hdma_uart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_uart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  
  if (HAL_DMA_Init(&hdma_uart2_rx) != HAL_OK)
  {
    Error_Handler();
  }
  
  __HAL_LINKDMA(&huart2, hdmarx, hdma_uart2_rx);
  
  // Configure DMA interrupt
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}
#endif

#ifdef USE_UART3
void UART3_Init(void){
  __HAL_RCC_USART3_CLK_ENABLE();
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
		
		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART16;
		RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

		HAL_NVIC_SetPriority(USART1_IRQn, 0, 3);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
#ifdef USE_UART2
  else if(huart->Instance == USART2){
		/* Enable USART2 clock */
		
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART234578;
		RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);

    // Enable DMA-based reception instead of interrupt-based
    // __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);

		HAL_NVIC_SetPriority(USART2_IRQn, 1, 2);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
#endif
#ifdef USE_UART3
  else if(huart->Instance == USART3){
		/* Enable USART3 clock */
		
    RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART234578;
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

#ifdef USE_UART2
// UART2 transmission functions for radar data communication
void UART2_Send_Data(uint8_t* data, uint8_t length) {
	HAL_UART_Transmit(&huart2, data, length, HAL_MAX_DELAY);
}

// UART2 transmission function with status return for error handling
HAL_StatusTypeDef UART2_Send_Data_WithStatus(uint8_t* data, uint8_t length) {
	return HAL_UART_Transmit(&huart2, data, length, HAL_MAX_DELAY);
}

/**
 * @brief Start DMA-based UART2 reception
 */
void UART2_StartDMAReception(void) {
    // Reset buffer state
    uart2_dma_data_ready = 0;
    uart2_dma_data_size = 0;
    
    // Start DMA reception with circular buffer
    if (HAL_UART_Receive_DMA(&huart2, uart2_dma_buffer, UART2_DMA_BUFFER_SIZE) != HAL_OK) {
        Error_Handler();
    }
    UART_Send_String("UART2 DMA reception started.\n");
}

/**
 * @brief Process received DMA data
 * @param data: Pointer to received data buffer
 * @param size: Size of received data
 */
void UART2_ProcessDMAData(uint8_t* data, uint16_t size) {
    // For now, just copy to the existing queue for compatibility
    for (uint16_t i = 0; i < size; i++) {
        if (!queue_is_full(&uart2_rx_queue)) {
            queue_enqueue(&uart2_rx_queue, data[i]);
        } else {
            // Queue is full, dequeue oldest data
            queue_dequeue(&uart2_rx_queue);
            queue_enqueue(&uart2_rx_queue, data[i]);
        }
    }
    
    // Set flag for main loop
    uart2_rx_flag = 1;
}

/**
 * @brief Get pointer to DMA buffer
 * @retval Pointer to DMA buffer
 */
uint8_t* UART2_GetDMABuffer(void) {
    return uart2_dma_buffer;
}

/**
 * @brief Get size of received DMA data
 * @retval Size of received data
 */
uint16_t UART2_GetDMADataSize(void) {
    return uart2_dma_data_size;
}

/**
 * @brief Reset DMA buffer state
 */
void UART2_ResetDMABuffer(void) {
    uart2_dma_data_ready = 0;
    uart2_dma_data_size = 0;
}

/**
 * @brief Check if DMA data is ready for processing
 * @retval 1 if data is ready, 0 otherwise
 */
uint8_t UART2_IsDMADataReady(void) {
    return uart2_dma_data_ready;
}
#endif

void USART1_Rx_ISR(UART_HandleTypeDef* huart){
	uart1_rx_flag = 1;
	HAL_UART_Receive(huart, (uint8_t*)&uart1_rx_data, 1, HAL_MAX_DELAY);
}

#ifdef USE_UART2
void USART2_Rx_ISR(UART_HandleTypeDef* huart){
	uart2_rx_flag = 1;
	// HAL_UART_Receive(huart, (uint8_t*)&uart2_rx_data, 1, HAL_MAX_DELAY);
  // This function is now used for DMA-based reception
  // Legacy interrupt-based processing can be added here if needed
}
#endif

#ifdef USE_UART3
void USART3_Rx_ISR(UART_HandleTypeDef* huart){
	uart3_rx_flag = 1;
	HAL_UART_Receive(huart, (uint8_t*)&uart3_rx_data, 1, HAL_MAX_DELAY);
  queue_enqueue(&uart3_rx_queue, uart3_rx_data);
}
#endif

#ifdef USE_UART2
/**
 * @brief DMA transfer complete callback for UART2 RX
 * @param hdma: DMA handle
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        // Calculate the amount of data received
        uint16_t received_size = UART2_DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);
        
        // Process the received data
        UART2_ProcessDMAData(uart2_dma_buffer, received_size);
        
        // Mark data as ready
        uart2_dma_data_ready = 1;
        uart2_dma_data_size = received_size;
        
        // Restart DMA reception for continuous operation
        HAL_UART_Receive_DMA(huart, uart2_dma_buffer, UART2_DMA_BUFFER_SIZE);
    }
}

/**
 * @brief DMA transfer half complete callback for UART2 RX
 * @param hdma: DMA handle
 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        // Process the first half of the buffer
        UART2_ProcessDMAData(uart2_dma_buffer, UART2_DMA_BUFFER_SIZE / 2);
        
        // Set flag to indicate data is available
        uart2_rx_flag = 1;
    }
}

/**
 * @brief DMA error callback for UART2 RX
 * @param hdma: DMA handle
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        // Restart DMA reception on error
        HAL_UART_Receive_DMA(huart, uart2_dma_buffer, UART2_DMA_BUFFER_SIZE);
    }
}
#endif
