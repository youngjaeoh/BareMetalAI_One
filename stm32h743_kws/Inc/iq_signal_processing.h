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

/* Exported types ------------------------------------------------------------*/
typedef struct {
    float32_t heart_rate_bpm;
    float32_t breathing_rate_bpm;
    float32_t heart_rate_confidence;
    float32_t breathing_rate_confidence;
} VitalSigns_t;

/* Exported constants --------------------------------------------------------*/
#define IQ_SAMPLE_RATE          2000    // 2kHz sampling rate
#define IQ_DATA_LENGTH          2000    // Fixed data length for I and Q arrays

// Heart rate filter parameters (0.8-3.0 Hz, 48-180 bpm)
#define HR_FILTER_LOW_FREQ      0.8f
#define HR_FILTER_HIGH_FREQ     3.0f
#define HR_FILTER_ORDER         2

// Breathing rate filter parameters (0.1-0.5 Hz, 6-30 bpm)
#define BR_FILTER_LOW_FREQ      0.1f
#define BR_FILTER_HIGH_FREQ     0.5f
#define BR_FILTER_ORDER         2

// FFT parameters
#define FFT_SIZE                2048
#define FFT_SIZE_LOG2           11      // log2(2048)

/* Exported macro ------------------------------------------------------------*/
#define IQ_FREQ_TO_BPM(freq)    ((freq) * 60.0f)
#define IQ_NYQUIST_FREQ         ((float32_t)IQ_SAMPLE_RATE / 2.0f)

/* Exported functions prototypes ---------------------------------------------*/
void IQ_SignalProcessing_Init(void);
HAL_StatusTypeDef IQ_ProcessVitalSigns(float32_t *i_data, float32_t *q_data, VitalSigns_t *vital_signs);

// Internal helper functions (fixed length: IQ_DATA_LENGTH = 2000)
HAL_StatusTypeDef IQ_ComputePhase(float32_t *i_data, float32_t *q_data, float32_t *phase_data);
HAL_StatusTypeDef IQ_UnwrapPhase(float32_t *phase_data);
HAL_StatusTypeDef IQ_RemoveDCTrend(float32_t *signal_data);
HAL_StatusTypeDef IQ_ApplyBandpassFilter(float32_t *input_data, float32_t *output_data, 
                                        float32_t low_freq, float32_t high_freq);
HAL_StatusTypeDef IQ_ComputeFFT(float32_t *input_data, float32_t *fft_output);
HAL_StatusTypeDef IQ_FindPeakFrequency(float32_t *fft_data, float32_t min_freq, float32_t max_freq, 
                                      float32_t *peak_freq, float32_t *peak_magnitude);

/* Exported variables --------------------------------------------------------*/
extern arm_rfft_fast_instance_f32 fft_instance;
extern arm_biquad_casd_df1_inst_f32 hr_filter_instance;
extern arm_biquad_casd_df1_inst_f32 br_filter_instance;

#ifdef __cplusplus
}
#endif

#endif /*__ IQ_SIGNAL_PROCESSING_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
