#ifndef __UART_H
#define __UART_H	

#ifdef __cplusplus 
extern "C"{
#endif

#include "main.h"
#include "string.h"

/* Private function prototypes -----------------------------------------------*/
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
extern volatile int uart1_rx_flag;
extern volatile char uart1_rx_data;
extern volatile int uart2_rx_flag;
extern volatile char uart2_rx_data;

extern void UART_Init(void);
extern void UART1_Init(void);
extern void UART2_Init(void);
extern void UART_Send_Char(char ch);
extern void UART_Send_String(char *str);
extern void UART2_Send_Data(uint8_t* data, uint8_t length);
extern HAL_StatusTypeDef UART2_Send_Data_WithStatus(uint8_t* data, uint8_t length);

extern void USART1_Rx_ISR(UART_HandleTypeDef* huart);
extern void USART2_Rx_ISR(UART_HandleTypeDef* huart);

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#ifdef __cplusplus 
}
#endif

#endif
