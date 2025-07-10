/**
  ******************************************************************************
  * @file    iq_signal_processing.h
  * @brief   IQ signal processing for heart rate and breathing rate detection
  *          using CMSIS DSP library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IQ_SIGNAL_PROCESSING_H__
#define __IQ_SIGNAL_PROCESSING_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include "float_queue.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
    float phase_std;         // Standard deviation of phase
    float final_quality;     // Final quality score (0-100)
} SignalQuality_t;

/* Exported constants --------------------------------------------------------*/
#define IQ_PROCESSING_SAMPLES   250     // Processing 250 samples (1000 bytes / 4 = 250 floats)

// Signal quality analysis parameters
#define VITAL_FREQ_MIN          0.1f    // Minimum vital sign frequency (Hz)
#define VITAL_FREQ_MAX          3.0f    // Maximum vital sign frequency (Hz)

// FFT parameters
#define FFT_SIZE                256     // FFT size for signal quality analysis
#define FFT_SIZE_LOG2           8       // log2(256)

/* Exported macro ------------------------------------------------------------*/
// No macros needed for current implementation

/* Exported functions prototypes ---------------------------------------------*/
void IQ_SignalProcessing_Init(void);
HAL_StatusTypeDef IQ_ProcessFloatQueues(FloatQueue *i_queue, FloatQueue *q_queue, SignalQuality_t *quality, float *i_data_out, float *q_data_out);

// Internal helper functions (processing 250 samples)
HAL_StatusTypeDef IQ_ComputePhase(float *i_data, float *q_data, float *phase_data, uint32_t length);
HAL_StatusTypeDef IQ_UnwrapPhase(float *phase_data, uint32_t length);
HAL_StatusTypeDef IQ_RemoveDCTrend(float *signal_data, uint32_t length);
HAL_StatusTypeDef IQ_NanToNum(float *signal_data, uint32_t length);
HAL_StatusTypeDef IQ_ComputeFFT(float *input_data, float *fft_output);

/* Exported variables --------------------------------------------------------*/
extern arm_rfft_fast_instance_f32 fft_instance;

#ifdef __cplusplus
}
#endif

#endif /*__ IQ_SIGNAL_PROCESSING_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
