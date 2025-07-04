#ifndef __THREAD_SPI_H
#define __THREAD_SPI_H

#include "stm32h7xx_hal.h"

#define THREAD_SPI_HEADER        0xA5
#define THREAD_SPI_CMD_SEND      0x01
#define THREAD_SPI_CMD_RECEIVE   0x02
#define THREAD_SPI_CMD_PING      0x03
#define THREAD_SPI_CMD_PONG      0x04
#define THREAD_SPI_CMD_STATUS    0x05

typedef struct {
    uint8_t header;      // 0x80
    uint8_t command;     // 명령어
    uint8_t length;      // 데이터 길이
    uint8_t data[32];    // 데이터 (최대 32바이트)
} Thread_SPI_Packet_t;

typedef struct {
    uint32_t uptime;     // 시스템 업타임 (ms)
    uint8_t status;      // 상태 플래그
    uint16_t free_memory; // 사용 가능한 메모리
    float cpu_temp;      // CPU 온도 (시뮬레이션)
} Thread_Status_Info_t;

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef Thread_SPI_SendSystemInfo(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef Thread_SPI_SendPacket(SPI_HandleTypeDef *hspi, uint8_t cmd, const uint8_t* data, uint8_t len);
HAL_StatusTypeDef Thread_SPI_ReceivePacket(SPI_HandleTypeDef *hspi, Thread_SPI_Packet_t* packet);
HAL_StatusTypeDef Thread_SPI_SendPing(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef Thread_SPI_SendStatus(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef Thread_SPI_ProcessReceivedData(Thread_SPI_Packet_t* packet);
void Thread_SPI_UpdateUptime(void);

#ifdef __cplusplus
}
#endif

#endif
