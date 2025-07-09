/**
  ******************************************************************************
  * @file    iq_data_converter.h
  * @brief   IQ data converter for SC12 to float format conversion
  *          Converts 152-byte radar packets to 50 I/Q float samples
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IQ_DATA_CONVERTER_H__
#define __IQ_DATA_CONVERTER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "queue.h"
#include "float_queue.h"
#include "arm_math.h"


/* Exported constants --------------------------------------------------------*/
// 레이더 패킷 관련
#define RADAR_PACKET_SIZE           152     // 레이더 패킷 전체 크기 (start+data+end)
#define RADAR_DATA_SIZE             150     // 실제 IQ 데이터 크기
#define RADAR_START_BYTE            0xEE    // 레이더 패킷 시작 바이트
#define RADAR_END_BYTE              0xAA    // 레이더 패킷 종료 바이트

// SC12 변환 관련
#define IQ_SC12_BYTES_PER_SAMPLE    3       // SC12 포맷: 3바이트당 1개 IQ 샘플
#define IQ_SAMPLES_PER_PACKET       50      // 패킷당 IQ 샘플 수 (150/3 = 50)

// I/Q Queue 관련 (핵심)
#define IQ_QUEUE_CAPACITY           500     // I/Q 큐 각각의 용량 (바이트)
#define IQ_BYTES_PER_SAMPLE         4       // float 1개당 4바이트
#define IQ_SAMPLES_FOR_PROCESSING   250     // 신호처리용 샘플 수 (500/2 = 250)

/* Exported macro ------------------------------------------------------------*/
#define IQ_SC12_TO_SC16_I(data0, data1)    (((data0) << 4) | ((data1) >> 4))
#define IQ_SC12_TO_SC16_Q(data1, data2)    (((data1) & 0x0F) << 8) | (data2)
#define IQ_SC16_TO_FLOAT(value)           ((float)((int16_t)(value) - 2048) / 2048.0f)

/* Exported functions prototypes ---------------------------------------------*/
HAL_StatusTypeDef IQ_ValidateRadarPacket(CircularQueue *input_queue);
HAL_StatusTypeDef IQ_ConvertQueueToIQQueues(CircularQueue *input_queue, FloatQueue *i_queue, FloatQueue *q_queue);
HAL_StatusTypeDef IQ_CheckIQQueuesReady(FloatQueue *i_queue, FloatQueue *q_queue);

/* Exported variables --------------------------------------------------------*/
// I/Q 큐들은 main.c에서 선언됩니다
// extern FloatQueue i_data_queue;
// extern FloatQueue q_data_queue;

#ifdef __cplusplus
}
#endif

#endif /*__ IQ_DATA_CONVERTER_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
