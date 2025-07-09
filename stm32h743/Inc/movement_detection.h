/**
  ******************************************************************************
  * @file    movement_detection.h
  * @brief   Movement level detection from IQ signal magnitude
  *          Processes complex signal to detect movement energy
  ******************************************************************************
  * @attention
  *
  * This module calculates movement level from I/Q data using magnitude
  * analysis and high-pass filtering, equivalent to Python implementation.
  *
  ******************************************************************************
  */

#ifndef __MOVEMENT_DETECTION_H
#define __MOVEMENT_DETECTION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "float_queue.h"
#include "arm_math.h"

/* Exported types ------------------------------------------------------------*/
// 단순화: normalized_movement 값만 반환
// typedef struct 제거하고 float 직접 반환

/* Exported constants --------------------------------------------------------*/
#define MOVEMENT_PROCESSING_SAMPLES  250    // Process 250 samples (same as signal quality)
#define MOVEMENT_CUTOFF_FREQ         5.0f   // High-pass filter cutoff frequency

// High-pass filter parameters (3rd order Butterworth at 5Hz with fs=50Hz)
#define MOVEMENT_FILTER_STAGES       2      // Cascaded biquad stages for 3rd order
#define MOVEMENT_FILTER_COEFFS       5      // Coefficients per stage

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
void Movement_Detection_Init(void);
float Movement_CalculateLevel(FloatQueue *i_queue, FloatQueue *q_queue);

// Internal helper functions
HAL_StatusTypeDef Movement_CalculateMagnitude(float *i_data, float *q_data, float *magnitude, uint32_t length);
HAL_StatusTypeDef Movement_RemoveDC(float *signal_data, uint32_t length);
HAL_StatusTypeDef Movement_ApplyHighPassFilter(float *input_data, float *output_data, uint32_t length);
HAL_StatusTypeDef Movement_NanToNum(float *signal_data, uint32_t length);

/* Exported variables --------------------------------------------------------*/
extern arm_biquad_casd_df1_inst_f32 movement_filter_instance;

#ifdef __cplusplus
}
#endif

#endif /* __MOVEMENT_DETECTION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
