#include "thread_spi.h"
#include <string.h>
#include <stdio.h>
#include "uart.h"

static uint32_t system_uptime = 0;
static float simulated_cpu_temp = 25.0f;

HAL_StatusTypeDef Thread_SPI_SendSystemInfo(SPI_HandleTypeDef *hspi) {
    char system_info[32];
    sprintf(system_info, "STM32H743_%lu", system_uptime);

    UART_Send_String("Sending System Info: ");
    UART_Send_String(system_info);
    UART_Send_String("\r\n");

    return Thread_SPI_SendPacket(hspi, THREAD_SPI_CMD_SEND, (uint8_t*)system_info, strlen(system_info));
}

HAL_StatusTypeDef Thread_SPI_SendPacket(SPI_HandleTypeDef *hspi, uint8_t cmd, const uint8_t* data, uint8_t len) {
    Thread_SPI_Packet_t packet;
    packet.header = THREAD_SPI_HEADER;
    packet.command = cmd;
    packet.length = len;

    if (len > 0 && data != NULL) {
        memcpy(packet.data, data, len);
    }

    // 패킷 내용 UART로 출력 (hex dump)
    char hex_str[128] = {0};
    int offset = 0;
    offset += sprintf(hex_str + offset, "TX Packet: ");
    offset += sprintf(hex_str + offset, "%02X ", packet.header);
    offset += sprintf(hex_str + offset, "%02X ", packet.command);
    offset += sprintf(hex_str + offset, "%02X ", packet.length);
    for (int i = 0; i < len; i++) {
        offset += sprintf(hex_str + offset, "%02X ", packet.data[i]);
    }
    offset += sprintf(hex_str + offset, "\r\n");
    UART_Send_String(hex_str);

    return HAL_SPI_Transmit(hspi, (uint8_t*)&packet, 3 + len, 1000);
}

HAL_StatusTypeDef Thread_SPI_ReceivePacket(SPI_HandleTypeDef *hspi, Thread_SPI_Packet_t* packet) {
    if (packet == NULL) return HAL_ERROR;

    // 패킷 수신 (헤더 + 명령어 + 길이 + 데이터)
    HAL_StatusTypeDef status = HAL_SPI_Receive(hspi, (uint8_t*)packet, 3, 1000);
    if (status != HAL_OK) return status;

    // 헤더 검증
    if (packet->header != THREAD_SPI_HEADER) return HAL_ERROR;

    // 데이터가 있으면 추가 수신
    if (packet->length > 0 && packet->length <= 32) {
        status = HAL_SPI_Receive(hspi, packet->data, packet->length, 1000);
    }

    return status;
}

HAL_StatusTypeDef Thread_SPI_SendPing(SPI_HandleTypeDef *hspi) {
    UART_Send_String("Sending PING\r\n");
    return Thread_SPI_SendPacket(hspi, THREAD_SPI_CMD_PING, NULL, 0);
}

HAL_StatusTypeDef Thread_SPI_SendStatus(SPI_HandleTypeDef *hspi) {
    Thread_Status_Info_t status_info;
    status_info.uptime = system_uptime;
    status_info.status = 0x01; // 정상 상태
    status_info.free_memory = 512; // 512KB (시뮬레이션)
    status_info.cpu_temp = simulated_cpu_temp; // 온도 변화 (시뮬레이션)

    char status_msg[128];
    sprintf(status_msg, "Sending Status - Uptime: %lu ms, Temp: %.1fC, Memory: %d KB\r\n",
            status_info.uptime, status_info.cpu_temp, status_info.free_memory);
    UART_Send_String(status_msg);

    return Thread_SPI_SendPacket(hspi, THREAD_SPI_CMD_STATUS, (uint8_t*)&status_info, sizeof(status_info));
}

HAL_StatusTypeDef Thread_SPI_ProcessReceivedData(Thread_SPI_Packet_t* packet) {
    if (packet == NULL) return HAL_ERROR;

    switch (packet->command) {
        case THREAD_SPI_CMD_PONG:
            // Pong 응답 처리
            UART_Send_String("Received PONG from slave\r\n");
            break;

        case THREAD_SPI_CMD_SEND:
            // 데이터 수신 처리
            UART_Send_String("Received data from slave: ");
            for (int i = 0; i < packet->length; i++) {
                char char_msg[2] = {packet->data[i], '\0'};
                UART_Send_String(char_msg);
            }
            UART_Send_String("\r\n");
            break;

        case THREAD_SPI_CMD_STATUS:
            // 상태 정보 수신 처리
            if (packet->length >= sizeof(Thread_Status_Info_t)) {
                Thread_Status_Info_t* slave_status = (Thread_Status_Info_t*)packet->data;
                char status_msg[128];
                sprintf(status_msg, "Slave Status - Uptime: %lu ms, Temp: %.1fC\r\n", 
                       slave_status->uptime, slave_status->cpu_temp);
                UART_Send_String(status_msg);
            }
            break;

        default:
            char error_msg[64];
            sprintf(error_msg, "Unknown command: 0x%02X\r\n", packet->command);
            UART_Send_String(error_msg);
            return HAL_ERROR;
    }

    return HAL_OK;
}

void Thread_SPI_UpdateUptime(void) {
    system_uptime += 1000;
    simulated_cpu_temp += (rand() % 10 - 5) * 0.1f; // 온도 변화 시뮬레이션
    if (simulated_cpu_temp < 20.0f) simulated_cpu_temp = 20.0f;
    if (simulated_cpu_temp > 30.0f) simulated_cpu_temp = 30.0f;
}
