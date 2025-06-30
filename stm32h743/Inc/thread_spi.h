#ifndef __THREAD_SPI_H
#define __THREAD_SPI_H

#include "stm32h7xx_hal.h"

#define THREAD_SPI_HEADER        0x80
#define THREAD_SPI_CMD_SEND      0x01

#ifdef __cplusplus
extern "C" {
#endif

HAL_StatusTypeDef Thread_SPI_SendHelloWorld(SPI_HandleTypeDef *hspi);

#ifdef __cplusplus
}
#endif

#endif 