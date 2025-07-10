/**
  ******************************************************************************
  * @file    radar_data_processor.h
  * @brief   Header file for radar data processing with DMA optimization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef __RADAR_DATA_PROCESSOR_H
#define __RADAR_DATA_PROCESSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "float_queue.h"

/* Radar packet configuration */
#define RADAR_PACKET_SIZE 152
#define RADAR_START_BYTE 0xAA
#define RADAR_END_BYTE 0x55
#define IQ_SAMPLES_PER_PACKET 50  // (152 - 2) / 3 = 50 samples

/* Function prototypes */
HAL_StatusTypeDef RadarData_ProcessDMABuffer(uint8_t* buffer, uint16_t size, 
                                           FloatQueue* i_queue, FloatQueue* q_queue);
HAL_StatusTypeDef RadarData_ProcessSinglePacket(uint8_t* packet, 
                                              FloatQueue* i_queue, FloatQueue* q_queue);
uint16_t RadarData_FindPacketInBuffer(uint8_t* buffer, uint16_t size, uint16_t start_pos);
HAL_StatusTypeDef RadarData_ValidatePacket(uint8_t* packet);

#ifdef __cplusplus
}
#endif

#endif /* __RADAR_DATA_PROCESSOR_H */
