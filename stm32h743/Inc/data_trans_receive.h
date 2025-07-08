/**
  ******************************************************************************
  * @file    data_trans_receive.h
  * @brief   This file contains all the function prototypes for
  *          the data transmission and reception between two STM32H743 boards
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATA_TRANS_RECEIVE_H__
#define __DATA_TRANS_RECEIVE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart.h"
#include "queue.h"
#include "lcd.h"
#include <string.h>

/* Exported types ------------------------------------------------------------*/
typedef enum {
    BOARD_A = 0,  // 마스터 보드 (Flag 전송, 데이터 수신 및 처리)
    BOARD_B = 1   // 슬레이브 보드 (Flag 수신, 데이터 전송)
} BoardMode_t;

typedef enum {
    MSG_PING = 0x01,
    MSG_PONG = 0x02,
    MSG_FLAG = 0x03,
    MSG_DATA = 0x04
} MessageType_t;

typedef struct {
    uint8_t start_byte;     // 시작 바이트 (0xAA)
    uint8_t msg_type;       // 메시지 타입
    uint8_t data;           // 데이터 (1바이트)
    uint8_t end_byte;       // 종료 바이트 (0x55)
} SimplePacket_t;

/* Exported constants --------------------------------------------------------*/
#define PACKET_START_BYTE   0xAA
#define PACKET_END_BYTE     0x55
#define DATA_SEND_INTERVAL  1000  // 1초마다 데이터 전송 (B보드)
#define PING_INTERVAL       2000  // 2초마다 ping 전송 (테스트용)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void DataTransReceive_Init(BoardMode_t mode);
void DataTransReceive_PingPongTest(void);
HAL_StatusTypeDef SendPacket(SimplePacket_t* packet);
HAL_StatusTypeDef ReceivePacket(SimplePacket_t* packet);

/* Exported variables --------------------------------------------------------*/
extern BoardMode_t current_board_mode;
extern CircularQueue uart3_rx_queue;
extern CircularQueue data_process_queue;
extern uint8_t flag_status;
extern uint16_t lcd_line;

#ifdef __cplusplus
}
#endif

#endif /*__ DATA_TRANS_RECEIVE_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
