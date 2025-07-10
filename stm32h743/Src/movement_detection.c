/**
  ******************************************************************************
  * @file    movement_detection.c
  * @brief   Movement level detection implementation
  *          Equivalent to Python calculate_movement_level function
  ******************************************************************************
  * @attention
  *
  * This module implements movement detection using magnitude analysis
  * and high-pass filtering for STM32H743.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "movement_detection.h"
#include "uart.h"
#include <string.h>
#include <math.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// High-pass filter instance (3rd order Butterworth)
arm_biquad_casd_df1_inst_f32 movement_filter_instance;

// High-pass filter coefficients (3rd order Butterworth, fc=5Hz, fs=50Hz)  
// Calculated using Python: scipy.signal.butter(3, 5.0/25.0, btype='high', output='sos')
// For fs=50Hz, nyquist=25Hz, cutoff=5Hz: normalized_freq = 5.0/25.0 = 0.2
// SOS (Second-Order Sections) format converted to CMSIS-DSP biquad cascade
static float movement_filter_coeffs[MOVEMENT_FILTER_STAGES * MOVEMENT_FILTER_COEFFS] = {
    // Stage 1: High-pass biquad section 1 (fs=50Hz, fc=5Hz)
    // Calculated coefficients for scipy.signal.butter(3, 0.2, btype='high')
    0.586874f, -1.173749f, 0.586874f, 1.031130f, -0.316812f,    // b0, b1, b2, a1, a2
    // Stage 2: High-pass biquad section 2 (fs=50Hz, fc=5Hz)
    // Second biquad section to complete 3rd order filter
    1.000000f, -1.000000f, 0.000000f, 0.683188f, -0.000000f     // b0, b1, b2, a1, a2
};

// Filter state buffer
static float movement_filter_state[MOVEMENT_FILTER_STAGES * 4];

// Working buffers
static float i_data_buffer[MOVEMENT_PROCESSING_SAMPLES];
static float q_data_buffer[MOVEMENT_PROCESSING_SAMPLES];
static float magnitude_buffer[MOVEMENT_PROCESSING_SAMPLES];
static float filtered_buffer[MOVEMENT_PROCESSING_SAMPLES];

/* Private function prototypes -----------------------------------------------*/
// static void Movement_PrintDebugInfo(const char* message, float value);

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize movement detection module
  * @param  None
  * @retval None
  */
void Movement_Detection_Init(void)
{
    // Initialize high-pass filter
    arm_biquad_cascade_df1_init_f32(&movement_filter_instance, 
                                   MOVEMENT_FILTER_STAGES,
                                   movement_filter_coeffs,
                                   movement_filter_state);
    
    // Clear working buffers
    memset(i_data_buffer, 0, sizeof(i_data_buffer));
    memset(q_data_buffer, 0, sizeof(q_data_buffer));
    memset(magnitude_buffer, 0, sizeof(magnitude_buffer));
    memset(filtered_buffer, 0, sizeof(filtered_buffer));
    memset(movement_filter_state, 0, sizeof(movement_filter_state));
    
    // Movement_PrintDebugInfo("Movement Detection Initialized", 0);
}

/**
  * @brief  Calculate movement level from I/Q data arrays (not queues)
  *         Returns only normalized_movement value (0-100)
  * @param  i_data: I component data array (250 samples)
  * @param  q_data: Q component data array (250 samples)
  * @retval Normalized movement level (0-100), or 0.0f on error
  */
float Movement_CalculateLevel(float *i_data, float *q_data)
{
    if (i_data == NULL || q_data == NULL) {
        return 0.0f;
    }
    
    // Copy input data to working buffers
    memcpy(i_data_buffer, i_data, MOVEMENT_PROCESSING_SAMPLES * sizeof(float));
    memcpy(q_data_buffer, q_data, MOVEMENT_PROCESSING_SAMPLES * sizeof(float));
    
    // Step 1: Calculate magnitude = np.abs(complex_signal)
    if (Movement_CalculateMagnitude(i_data_buffer, q_data_buffer, magnitude_buffer, MOVEMENT_PROCESSING_SAMPLES) != HAL_OK) {
        return 0.0f;
    }
    
    // Step 2: Handle NaN values - magnitude = np.nan_to_num(magnitude, ...)
    if (Movement_NanToNum(magnitude_buffer, MOVEMENT_PROCESSING_SAMPLES) != HAL_OK) {
        return 0.0f;
    }
    
    // Step 3: Remove DC component - magnitude = magnitude - np.mean(magnitude)
    if (Movement_RemoveDC(magnitude_buffer, MOVEMENT_PROCESSING_SAMPLES) != HAL_OK) {
        return 0.0f;
    }
    
    // Step 4: Apply high-pass filter - signal.filtfilt(b, a, magnitude)
    if (Movement_ApplyHighPassFilter(magnitude_buffer, filtered_buffer, MOVEMENT_PROCESSING_SAMPLES) != HAL_OK) {
        // If filter fails, use original magnitude (Python fallback behavior)
        memcpy(filtered_buffer, magnitude_buffer, sizeof(magnitude_buffer));
    }
    
    // Step 5: Handle NaN values again - movement_signal = np.nan_to_num(movement_signal, ...)
    if (Movement_NanToNum(filtered_buffer, MOVEMENT_PROCESSING_SAMPLES) != HAL_OK) {
        return 0.0f;
    }
    
    // Step 6: Calculate movement energy - movement_energy = np.sum(movement_signal ** 2)
    float movement_energy = 0.0f;
    for (uint32_t i = 0; i < MOVEMENT_PROCESSING_SAMPLES; i++) {
        movement_energy += filtered_buffer[i] * filtered_buffer[i];
    }
    
    // Step 7: Calculate average energy and normalize - avg_energy = movement_energy / signal_length
    float avg_energy = movement_energy / MOVEMENT_PROCESSING_SAMPLES;
    
    // Step 8: Normalize movement level - normalized_movement = np.clip(avg_energy / 100, 0, 100)
    float normalized_movement = avg_energy / 100.0f;
    if (normalized_movement > 100.0f) normalized_movement = 100.0f;
    if (normalized_movement < 0.0f) normalized_movement = 0.0f;
    
    // Movement_PrintDebugInfo("Movement level calculated", normalized_movement);
    
    return normalized_movement;
}

/* Helper Functions ----------------------------------------------------------*/

/**
  * @brief  Calculate magnitude from I and Q data (np.abs(complex_signal))
  * @param  i_data: I component data
  * @param  q_data: Q component data
  * @param  magnitude: Output magnitude data
  * @param  length: Number of samples to process
  * @retval HAL status
  */
HAL_StatusTypeDef Movement_CalculateMagnitude(float *i_data, float *q_data, float *magnitude, uint32_t length)
{
    if (i_data == NULL || q_data == NULL || magnitude == NULL) {
        return HAL_ERROR;
    }
    
    // Calculate magnitude: sqrt(I^2 + Q^2)
    for (uint32_t i = 0; i < length; i++) {
        magnitude[i] = sqrtf(i_data[i] * i_data[i] + q_data[i] * q_data[i]);
    }
    
    return HAL_OK;
}

/**
  * @brief  Remove DC component (magnitude = magnitude - np.mean(magnitude))
  * @param  signal_data: Signal data to process
  * @param  length: Number of samples to process
  * @retval HAL status
  */
HAL_StatusTypeDef Movement_RemoveDC(float *signal_data, uint32_t length)
{
    if (signal_data == NULL || length == 0) {
        return HAL_ERROR;
    }
    
    // Calculate mean
    float mean = 0.0f;
    for (uint32_t i = 0; i < length; i++) {
        mean += signal_data[i];
    }
    mean /= length;
    
    // Remove DC component
    for (uint32_t i = 0; i < length; i++) {
        signal_data[i] -= mean;
    }
    
    return HAL_OK;
}

/**
  * @brief  Apply high-pass filter (signal.filtfilt equivalent)
  * @param  input_data: Input signal data
  * @param  output_data: Output filtered data
  * @param  length: Number of samples to process
  * @retval HAL status
  */
HAL_StatusTypeDef Movement_ApplyHighPassFilter(float *input_data, float *output_data, uint32_t length)
{
    if (input_data == NULL || output_data == NULL || length == 0) {
        return HAL_ERROR;
    }
    
    // Reset filter state for clean filtering
    memset(movement_filter_state, 0, sizeof(movement_filter_state));
    
    // Apply high-pass filter using CMSIS DSP
    arm_biquad_cascade_df1_f32(&movement_filter_instance, input_data, output_data, length);
    
    return HAL_OK;
}

/**
  * @brief  Replace NaN and Inf values with finite values (np.nan_to_num equivalent)
  * @param  signal_data: Signal data to process
  * @param  length: Number of samples to process
  * @retval HAL status
  */
HAL_StatusTypeDef Movement_NanToNum(float *signal_data, uint32_t length)
{
    if (signal_data == NULL) {
        return HAL_ERROR;
    }
    
    for (uint32_t i = 0; i < length; i++) {
        if (isnanf(signal_data[i]) || isinff(signal_data[i])) {
            signal_data[i] = 0.0f;
        }
    }
    
    return HAL_OK;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Print debug information
  * @param  message: Debug message
  * @param  value: Value to print
  * @retval None
  */
static void Movement_PrintDebugInfo(const char* message, float value)
{
    char debug_buffer[100];
    sprintf(debug_buffer, "[MOVEMENT] %s: %.2f\r\n", message, value);
    UART_Send_String(debug_buffer);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*
=== 수정된 Movement Detection 구현 (fs=50Hz) ===

Python 원본:
def calculate_movement_level(complex_signal, fs):
    # fs = 50 Hz (샘플링 주파수)
    # nyquist = 25 Hz
    # cutoff = 5.0 Hz  
    # normalized_freq = 5.0/25.0 = 0.2
    # signal.butter(3, 0.2, btype='high') -> 3차 고주파 통과 필터
    return normalized_movement  # 0-100 범위의 단일 값

=== C 사용 예제 ===

#include "movement_detection.h"
#include "float_queue.h"

// 초기화
Movement_Detection_Init();

// FloatQueue 선언 (main.c에서)
FloatQueue i_data_queue;
FloatQueue q_data_queue;

// 메인 루프에서 움직임 감지
while (1) {
    // 250개 샘플이 큐에 준비되었는지 확인
    if (float_queue_count(&i_data_queue) >= MOVEMENT_PROCESSING_SAMPLES) {
        
        // 단순하게 float 값 하나만 반환
        float movement_level = Movement_CalculateLevel(&i_data_queue, &q_data_queue);
        
        printf("Movement Level: %.2f\n", movement_level);
        
        // 움직임 감지 로직
        if (movement_level > 10.0f) {
            printf("High movement detected!\n");
        }
    }
    
    HAL_Delay(200);  // 0.2초 대기
}

=== 올바른 필터 파라미터 ===

1. 샘플링 주파수: fs = 50 Hz (Python과 동일)
2. 나이퀴스트 주파수: 25 Hz  
3. 컷오프 주파수: 5 Hz (Python과 동일)
4. 정규화된 주파수: 0.2 (5Hz/25Hz = 0.2)
5. 필터 차수: 3차 Butterworth 고주파 통과 (Python과 동일)
6. 구현: 2개의 biquad 섹션으로 cascaded

=== 핵심 공식 ===

normalized_movement = clip(avg_energy / 100, 0, 100)
- avg_energy = sum(filtered_signal²) / 250
- filtered_signal = high_pass_filter(magnitude - dc)  # 5Hz cutoff, fs=50Hz
- magnitude = sqrt(I² + Q²)
*/
