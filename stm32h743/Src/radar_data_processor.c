/**
  ******************************************************************************
  * @file    radar_data_processor.c
  * @brief   Radar data processing with DMA optimization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

#include "radar_data_processor.h"
#include "iq_data_converter.h"
#include "uart.h"
#include <string.h>

/* Private function prototypes */
static float SC16_to_float(uint16_t sc16_value);
static uint16_t SC12_to_SC16_I(uint8_t data0, uint8_t data1);
static uint16_t SC12_to_SC16_Q(uint8_t data1, uint8_t data2);

/**
 * @brief Process DMA buffer containing radar data
 * @param buffer: DMA buffer containing radar data
 * @param size: Size of the buffer
 * @param i_queue: Queue for I component data
 * @param q_queue: Queue for Q component data
 * @retval HAL status
 */
HAL_StatusTypeDef RadarData_ProcessDMABuffer(uint8_t* buffer, uint16_t size, 
                                           FloatQueue* i_queue, FloatQueue* q_queue)
{
    if (buffer == NULL || i_queue == NULL || q_queue == NULL) {
        return HAL_ERROR;
    }
    
    uint16_t pos = 0;
    uint16_t packets_processed = 0;
    
    // Process all complete packets in the buffer
    while (pos < size) {
        uint16_t packet_start = RadarData_FindPacketInBuffer(buffer, size, pos);
        
        if (packet_start == 0xFFFF) {
            // No more packets found
            break;
        }
        
        // Check if we have a complete packet
        if (packet_start + RADAR_PACKET_SIZE <= size) {
            if (RadarData_ProcessSinglePacket(&buffer[packet_start], i_queue, q_queue) == HAL_OK) {
                packets_processed++;
            }
            pos = packet_start + RADAR_PACKET_SIZE;
        } else {
            // Incomplete packet at end of buffer
            break;
        }
    }
    
    if (packets_processed > 0) {
        char debug_msg[50];
        sprintf(debug_msg, "Processed %d radar packets\r\n", packets_processed);
        UART_Send_String(debug_msg);
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

/**
 * @brief Process a single radar packet
 * @param packet: Pointer to packet data (152 bytes)
 * @param i_queue: Queue for I component data
 * @param q_queue: Queue for Q component data
 * @retval HAL status
 */
HAL_StatusTypeDef RadarData_ProcessSinglePacket(uint8_t* packet, 
                                              FloatQueue* i_queue, FloatQueue* q_queue)
{
    if (packet == NULL || i_queue == NULL || q_queue == NULL) {
        return HAL_ERROR;
    }
    
    // Validate packet
    if (RadarData_ValidatePacket(packet) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Process I/Q samples (skip start byte, process 150 bytes, skip end byte)
    for (uint32_t sample_idx = 0; sample_idx < IQ_SAMPLES_PER_PACKET; sample_idx++) {
        uint32_t byte_offset = 1 + (sample_idx * 3);  // Skip start byte
        
        uint8_t data0 = packet[byte_offset];
        uint8_t data1 = packet[byte_offset + 1];
        uint8_t data2 = packet[byte_offset + 2];
        
        // Convert SC12 to SC16
        uint16_t i_value = SC12_to_SC16_I(data0, data1);
        uint16_t q_value = SC12_to_SC16_Q(data1, data2);
        
        // Convert to float and enqueue
        float_queue_enqueue(i_queue, SC16_to_float(i_value));
        float_queue_enqueue(q_queue, SC16_to_float(q_value));
    }
    
    return HAL_OK;
}

/**
 * @brief Find the start of a radar packet in buffer
 * @param buffer: Buffer to search
 * @param size: Size of buffer
 * @param start_pos: Starting position for search
 * @retval Position of packet start, or 0xFFFF if not found
 */
uint16_t RadarData_FindPacketInBuffer(uint8_t* buffer, uint16_t size, uint16_t start_pos)
{
    if (buffer == NULL || start_pos >= size) {
        return 0xFFFF;
    }
    
    for (uint16_t i = start_pos; i < size; i++) {
        if (buffer[i] == RADAR_START_BYTE) {
            // Check if we have enough bytes for end byte
            if (i + RADAR_PACKET_SIZE - 1 < size) {
                if (buffer[i + RADAR_PACKET_SIZE - 1] == RADAR_END_BYTE) {
                    return i;
                }
            }
        }
    }
    
    return 0xFFFF;
}

/**
 * @brief Validate a radar packet
 * @param packet: Pointer to packet data
 * @retval HAL status
 */
HAL_StatusTypeDef RadarData_ValidatePacket(uint8_t* packet)
{
    if (packet == NULL) {
        return HAL_ERROR;
    }
    
    if (packet[0] != RADAR_START_BYTE) {
        return HAL_ERROR;
    }
    
    if (packet[RADAR_PACKET_SIZE - 1] != RADAR_END_BYTE) {
        return HAL_ERROR;
    }
    
    return HAL_OK;
}

/* Private functions */

/**
 * @brief Convert SC16 to float
 * @param sc16_value: SC16 value to convert
 * @retval Converted float value
 */
static float SC16_to_float(uint16_t sc16_value)
{
    int16_t signed_value = (int16_t)sc16_value;
    if (signed_value >= 2048) {
        signed_value -= 4096;  // Convert to signed 12-bit
    }
    return (float)signed_value / 2048.0f;
}

/**
 * @brief Convert SC12 to SC16 for I component
 * @param data0: First byte
 * @param data1: Second byte
 * @retval SC16 I value
 */
static uint16_t SC12_to_SC16_I(uint8_t data0, uint8_t data1)
{
    return (uint16_t)((data0 << 4) | (data1 >> 4));
}

/**
 * @brief Convert SC12 to SC16 for Q component
 * @param data1: Second byte
 * @param data2: Third byte
 * @retval SC16 Q value
 */
static uint16_t SC12_to_SC16_Q(uint8_t data1, uint8_t data2)
{
    return (uint16_t)(((data1 & 0x0F) << 8) | data2);
}
