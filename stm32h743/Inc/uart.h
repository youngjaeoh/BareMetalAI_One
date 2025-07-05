#ifndef __UART_H
#define __UART_H	

#ifdef __cplusplus 
extern "C"{
#endif

#include "main.h"
#include "string.h"

/* UART Configuration Defines -----------------------------------------------*/
// UART2를 사용하려면 아래 주석을 해제하세요
// #define USE_UART2
// UART3를 사용하려면 아래 주석을 해제하세요
// #define USE_UART3

/* Private function prototypes -----------------------------------------------*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

/* UART1 related declarations */
extern volatile int uart1_rx_flag;
extern volatile char uart1_rx_data;
extern UART_HandleTypeDef huart1;
extern void USART1_Rx_ISR(UART_HandleTypeDef* huart);

/* UART2 related declarations - only if USE_UART2 is defined */
#ifdef USE_UART2
extern volatile int uart2_rx_flag;
extern volatile char uart2_rx_data;
extern UART_HandleTypeDef huart2;
extern void UART2_Init(void);
extern void UART2_Send_Data(uint8_t* data, uint8_t length);
extern HAL_StatusTypeDef UART2_Send_Data_WithStatus(uint8_t* data, uint8_t length);
extern void USART2_Rx_ISR(UART_HandleTypeDef* huart);
#endif

/* UART3 related declarations - only if USE_UART3 is defined */
#ifdef USE_UART3
extern volatile int uart3_rx_flag;
extern volatile char uart3_rx_data;
extern UART_HandleTypeDef huart3;
extern void UART3_Init(void);
extern void UART3_Send_Data(uint8_t* data, uint8_t length);
extern HAL_StatusTypeDef UART3_Send_Data_WithStatus(uint8_t* data, uint8_t length);
extern void USART3_Rx_ISR(UART_HandleTypeDef* huart);
#endif

/* Common UART functions */
extern void UART_Init(void);
extern void UART1_Init(void);
extern void UART_Send_Char(char ch);
extern void UART_Send_String(char *str);

#ifdef __cplusplus 
}
#endif

#endif
