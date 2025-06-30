#include "thread_spi.h"
#include <string.h>

HAL_StatusTypeDef Thread_SPI_SendHelloWorld(SPI_HandleTypeDef *hspi) {
    const char msg[] = "Hello World";
    uint8_t len = sizeof(msg) - 1; // null 제외
    uint8_t packet[32];
    packet[0] = THREAD_SPI_HEADER;
    packet[1] = THREAD_SPI_CMD_SEND;
    packet[2] = len;
    memcpy(&packet[3], msg, len);
    return HAL_SPI_Transmit(hspi, packet, 3 + len, 100);
} 