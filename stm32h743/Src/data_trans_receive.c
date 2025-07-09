/**
  ******************************************************************************
  * @file    data_trans_receive.c
  * @brief   This file provides code for data transmission and reception
  *          between two STM32H743 boards using UART3
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
#include "data_trans_receive.h"
#include "st7735.h"

/* External variables --------------------------------------------------------*/
extern ST7735_LCD_Drv_t   ST7735_LCD_Driver;
extern ST7735_Object_t st7735_pObj;
extern ST7735_Ctx_t ST7735Ctx;

/* Private variables ---------------------------------------------------------*/
BoardMode_t current_board_mode;
CircularQueue uart3_rx_queue;
CircularQueue data_process_queue;
uint8_t flag_status = 0;
uint16_t lcd_line = 0;

// 테스트 모드 변수
static uint32_t last_ping_time = 0;
static uint32_t last_data_send_time = 0;
static uint8_t test_data_counter = 0;
static uint8_t is_ping_pong_mode = 0;

/* Private function prototypes -----------------------------------------------*/
static void LCD_DisplayMessage(char* message);
static void LCD_ClearAndReset(void);

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize data transmission and reception module
  * @param  mode: Board mode (BOARD_A or BOARD_B)
  * @retval None
  */
void DataTransReceive_Init(BoardMode_t mode)
{
    current_board_mode = mode;
    char debug_msg[50];
    
    // Initialize queues
    queue_init(&uart3_rx_queue);
    queue_init(&data_process_queue);
    
    // Debug: Print board mode
    // sprintf(debug_msg, "Board Mode: %s\r\n", (mode == BOARD_A) ? "MASTER (A)" : "SLAVE (B)");
    // UART_Send_String(debug_msg);
    
    // Initialize LCD display
    LCD_ClearAndReset();
    
    // Display initialization message
    if (mode == BOARD_A) {
        LCD_DisplayMessage("Board A Ready");
        LCD_DisplayMessage("- Send Flag");
        LCD_DisplayMessage("- Receive Data");
    } else {
        LCD_DisplayMessage("Board B Ready");
        LCD_DisplayMessage("- Receive Flag");
        LCD_DisplayMessage("- Send Data");
    }
    
    flag_status = 0;
}

/**
  * @brief  Ping-Pong test function
  * @param  None
  * @retval None
  */
void DataTransReceive_PingPongTest(void)
{
    uint32_t current_time = HAL_GetTick();
    SimplePacket_t packet;
    char debug_msg[50];
    
    is_ping_pong_mode = 1;
    
    // Process received packets
    if (ReceivePacket(&packet) == HAL_OK) {
        if (packet.msg_type == MSG_PING) {
            LCD_DisplayMessage("Received PING");
            UART_Send_String("*** PING Received ***\r\n");
            
            // Send PONG response
            packet.start_byte = PACKET_START_BYTE;
            packet.msg_type = MSG_PONG;
            packet.data = 0;
            packet.end_byte = PACKET_END_BYTE;
            
            SendPacket(&packet);
            LCD_DisplayMessage("Sent PONG");
            UART_Send_String("*** PONG Sent ***\r\n");
        }
        else if (packet.msg_type == MSG_PONG) {
            LCD_DisplayMessage("Received PONG");
            UART_Send_String("*** PONG Received ***\r\n");
        }
    }
    
    // Board A sends PING periodically
    if (current_board_mode == BOARD_A) {
        if (current_time - last_ping_time >= PING_INTERVAL) {
            packet.start_byte = PACKET_START_BYTE;
            packet.msg_type = MSG_PING;
            packet.data = 0;
            packet.end_byte = PACKET_END_BYTE;
            
            SendPacket(&packet);
            LCD_DisplayMessage("Sent PING");
            UART_Send_String("*** PING Sent ***\r\n");
            last_ping_time = current_time;
        }
    }
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Display message on LCD
  * @param  message: Message to display
  * @retval None
  */
static void LCD_DisplayMessage(char* message)
{
    // LCD 라인 관리 (8줄까지 표시)
    if (lcd_line >= 8) {
        LCD_ClearAndReset();
    }
    
    // 메시지 표시
    LCD_ShowString(0, lcd_line * 16, 128, 16, 12, (uint8_t*)message);
    lcd_line++;
}

/**
  * @brief  Clear LCD and reset line counter
  * @param  None
  * @retval None
  */
static void LCD_ClearAndReset(void)
{
    ST7735_LCD_Driver.FillRect(&st7735_pObj, 0, 0, ST7735Ctx.Width, ST7735Ctx.Height, BLACK);
    lcd_line = 0;
}

/**
  * @brief  Send packet via UART3 (4-byte packet, no checksum)
  * @param  packet: Packet to send
  * @retval HAL status
  */
HAL_StatusTypeDef SendPacket(SimplePacket_t* packet)
{
    uint8_t buffer[4];
    char debug_msg[50];
    
    buffer[0] = packet->start_byte;
    buffer[1] = packet->msg_type;
    buffer[2] = packet->data;
    buffer[3] = packet->end_byte;
    
    // Debug message via UART1
    //sprintf(debug_msg, "TX: %02X %02X %02X %02X\r\n", 
    //        buffer[0], buffer[1], buffer[2], buffer[3]);
    //UART_Send_String(debug_msg);
    
    return UART3_Send_Data_WithStatus(buffer, 4);
}

/**
  * @brief  Receive packet from UART3 (4-byte packet, no checksum)
  * @param  packet: Packet buffer to store received data
  * @retval HAL status
  */
HAL_StatusTypeDef ReceivePacket(SimplePacket_t* packet)
{
    uint8_t byte;
    static uint8_t state = 0;
    char debug_msg[50];
    
    while (!queue_is_empty(&uart3_rx_queue)) {
        byte = queue_dequeue(&uart3_rx_queue);
        
        // Debug: Print received byte
        //sprintf(debug_msg, "RX Byte: %02X (State: %d)\r\n", byte, state);
        //UART_Send_String(debug_msg);
        
        switch (state) {
            case 0: // Wait for start byte
                if (byte == PACKET_START_BYTE) {
                    packet->start_byte = byte;
                    state = 1;
                }
                break;
                
            case 1: // Receive message type
                packet->msg_type = byte;
                state = 2;
                break;
                
            case 2: // Receive data
                packet->data = byte;
                state = 3;
                break;
                
            case 3: // Receive end byte
                packet->end_byte = byte;
                state = 0; // Reset for next packet
                
                if (byte == PACKET_END_BYTE) {
                    //sprintf(debug_msg, "RX Complete: %02X %02X %02X %02X\r\n", 
                    //        packet->start_byte, packet->msg_type, packet->data, packet->end_byte);
                    //UART_Send_String(debug_msg);
                    //UART_Send_String("*** PACKET RECEIVED ***\r\n");
                    return HAL_OK;
                } else {
                    //UART_Send_String("*** Invalid end byte ***\r\n");
                    for(;;); // Halt on invalid end byte
                }
                break;
        }
    }
    
    return HAL_ERROR;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
