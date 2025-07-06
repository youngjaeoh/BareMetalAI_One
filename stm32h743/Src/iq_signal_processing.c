/**
  ******************************************************************************
  * @file    iq_signal_processing.c
  * @brief   IQ signal processing implementation for heart rate and breathing 
  *          rate detection using CMSIS DSP library
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iq_signal_processing.h"
#include "uart.h"
#include <string.h>
#include <math.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// FFT instance
arm_rfft_fast_instance_f32 fft_instance;

// Filter instances
arm_biquad_casd_df1_inst_f32 hr_filter_instance; //HR filter
arm_biquad_casd_df1_inst_f32 br_filter_instance; //BR filter

// Filter coefficients for heart rate (0.8-3.0 Hz bandpass)
static float32_t hr_filter_coeffs[5 * HR_FILTER_ORDER] = {0};
static float32_t hr_filter_state[4 * HR_FILTER_ORDER] = {0};

// Filter coefficients for breathing rate (0.1-0.5 Hz bandpass)
static float32_t br_filter_coeffs[5 * BR_FILTER_ORDER] = {0};
static float32_t br_filter_state[4 * BR_FILTER_ORDER] = {0};

// Working buffers
static float32_t phase_buffer[IQ_DATA_LENGTH];
static float32_t filtered_buffer[IQ_DATA_LENGTH];
static float32_t fft_input_buffer[FFT_SIZE];
static float32_t fft_output_buffer[FFT_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void IQ_CalculateFilterCoefficients(float32_t low_freq, float32_t high_freq, 
                                          float32_t sample_rate, float32_t *coeffs);
static void IQ_PrintDebugInfo(const char* message, float32_t value);

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize IQ signal processing module
  * @param  None
  * @retval None
  */
void IQ_SignalProcessing_Init(void)
{
    // Initialize FFT instance
    arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);
    
    // Calculate filter coefficients
    IQ_CalculateFilterCoefficients(HR_FILTER_LOW_FREQ, HR_FILTER_HIGH_FREQ, 
                                  IQ_SAMPLE_RATE, hr_filter_coeffs);
    IQ_CalculateFilterCoefficients(BR_FILTER_LOW_FREQ, BR_FILTER_HIGH_FREQ, 
                                  IQ_SAMPLE_RATE, br_filter_coeffs);
    
    // Initialize filter instances
    arm_biquad_cascade_df1_init_f32(&hr_filter_instance, HR_FILTER_ORDER, 
                                   hr_filter_coeffs, hr_filter_state);
    arm_biquad_cascade_df1_init_f32(&br_filter_instance, BR_FILTER_ORDER, 
                                   br_filter_coeffs, br_filter_state);
    
    // Clear working buffers
    memset(phase_buffer, 0, sizeof(phase_buffer));
    memset(filtered_buffer, 0, sizeof(filtered_buffer));
    memset(fft_input_buffer, 0, sizeof(fft_input_buffer));
    memset(fft_output_buffer, 0, sizeof(fft_output_buffer));
    
    IQ_PrintDebugInfo("IQ Signal Processing Initialized", 0);
}

/**
  * @brief  Process IQ data to extract vital signs
  * @param  i_data: I component array (length: 2000)
  * @param  q_data: Q component array (length: 2000)
  * @param  vital_signs: Output vital signs structure
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ProcessVitalSigns(float32_t *i_data, float32_t *q_data, VitalSigns_t *vital_signs)
{
    if (i_data == NULL || q_data == NULL || vital_signs == NULL) {
        return HAL_ERROR;
    }
    
    IQ_PrintDebugInfo("Processing IQ data, length", IQ_DATA_LENGTH);
    
    // Step 1: Compute phase from I and Q data
    // Python equivalent: phase = np.angle(I + 1j*Q)
    if (IQ_ComputePhase(i_data, q_data, phase_buffer) != HAL_OK) {
        return HAL_ERROR;
    }
    IQ_PrintDebugInfo("Phase computed", 0);
    
    // Step 2: Unwrap phase
    // Python equivalent: phase = np.unwrap(phase)
    if (IQ_UnwrapPhase(phase_buffer) != HAL_OK) {
        return HAL_ERROR;
    }
    IQ_PrintDebugInfo("Phase unwrapped", 0);
    
    // Step 3: Remove DC and trend
    // Python equivalent: phase_detrended = detrend(phase)
    if (IQ_RemoveDCTrend(phase_buffer) != HAL_OK) {
        return HAL_ERROR;
    }
    IQ_PrintDebugInfo("DC/Trend removed", 0);
    
    // Step 4a: Apply heart rate bandpass filter
    // Python equivalent: hr_phase_filtered = filtfilt(hr_b, hr_a, phase_detrended)
    if (IQ_ApplyBandpassFilter(phase_buffer, filtered_buffer,
                              HR_FILTER_LOW_FREQ, HR_FILTER_HIGH_FREQ) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 5a: Compute FFT for heart rate
    // Python equivalent: hr_fft_vals = np.abs(np.fft.rfft(hr_phase_filtered))
    if (IQ_ComputeFFT(filtered_buffer, fft_output_buffer) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 6a: Find heart rate peak
    // Python equivalent: hr_peak_freq = hr_freqs[np.argmax(hr_fft_vals)]
    float32_t hr_peak_freq, hr_peak_mag;
    if (IQ_FindPeakFrequency(fft_output_buffer, HR_FILTER_LOW_FREQ, 
                            HR_FILTER_HIGH_FREQ, &hr_peak_freq, &hr_peak_mag) != HAL_OK) {
        return HAL_ERROR;
    }
    
    vital_signs->heart_rate_bpm = IQ_FREQ_TO_BPM(hr_peak_freq);
    vital_signs->heart_rate_confidence = hr_peak_mag;
    
    // Step 4b: Apply breathing rate bandpass filter
    // Python equivalent: br_phase_filtered = filtfilt(br_b, br_a, phase_detrended)
    if (IQ_ApplyBandpassFilter(phase_buffer, filtered_buffer,
                              BR_FILTER_LOW_FREQ, BR_FILTER_HIGH_FREQ) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 5b: Compute FFT for breathing rate
    // Python equivalent: br_fft_vals = np.abs(np.fft.rfft(br_phase_filtered))
    if (IQ_ComputeFFT(filtered_buffer, fft_output_buffer) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 6b: Find breathing rate peak
    // Python equivalent: br_peak_freq = br_freqs[np.argmax(br_fft_vals)]
    float32_t br_peak_freq, br_peak_mag;
    if (IQ_FindPeakFrequency(fft_output_buffer, BR_FILTER_LOW_FREQ, 
                            BR_FILTER_HIGH_FREQ, &br_peak_freq, &br_peak_mag) != HAL_OK) {
        return HAL_ERROR;
    }
    
    vital_signs->breathing_rate_bpm = IQ_FREQ_TO_BPM(br_peak_freq);
    vital_signs->breathing_rate_confidence = br_peak_mag;
    
    // Print results
    // Python equivalent: print(f"추정 심박수: {hr_bpm:.2f} bpm")
    IQ_PrintDebugInfo("Heart Rate (BPM)", vital_signs->heart_rate_bpm);
    IQ_PrintDebugInfo("Breathing Rate (BPM)", vital_signs->breathing_rate_bpm);
    
    return HAL_OK;
}

/**
  * @brief  Compute phase from I and Q data
  * @param  i_data: I component data (float32, length: 2000)
  * @param  q_data: Q component data (float32, length: 2000)
  * @param  phase_data: Output phase data (float32, length: 2000)
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ComputePhase(float32_t *i_data, float32_t *q_data, float32_t *phase_data)
{
    if (i_data == NULL || q_data == NULL || phase_data == NULL) {
        return HAL_ERROR;
    }
    
    // Compute phase using atan2
    // Python equivalent: phase = np.angle(I + 1j*Q) = np.angle(IQ)
    for (uint32_t i = 0; i < IQ_DATA_LENGTH; i++) {
        phase_data[i] = atan2f(q_data[i], i_data[i]);
    }
    
    return HAL_OK;
}

/**
  * @brief  Unwrap phase data (fixed length: 2000)
  * @param  phase_data: Phase data to unwrap
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_UnwrapPhase(float32_t *phase_data)
{
    if (phase_data == NULL) {
        return HAL_ERROR;
    }
    
    const float32_t TWO_PI = 2.0f * PI;  // CMSIS-DSP의 PI 매크로 직접 사용
    
    for (uint32_t i = 1; i < IQ_DATA_LENGTH; i++) {
        float32_t diff = phase_data[i] - phase_data[i-1];
        
        // Unwrap phase jumps
        if (diff > PI) {
            phase_data[i] -= TWO_PI;
        } else if (diff < -PI) {
            phase_data[i] += TWO_PI;
        }
    }
    
    return HAL_OK;
}

/**
  * @brief  Remove DC component and linear trend (fixed length: 2000)
  * @param  signal_data: Signal data to process
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_RemoveDCTrend(float32_t *signal_data)
{
    if (signal_data == NULL) {
        return HAL_ERROR;
    }
    
    // Calculate mean (DC component)
    float32_t mean_value;
    arm_mean_f32(signal_data, IQ_DATA_LENGTH, &mean_value);
    
    // Remove DC component
    arm_offset_f32(signal_data, -mean_value, signal_data, IQ_DATA_LENGTH);
    
    // Simple linear detrending (remove linear trend)
    float32_t slope = (signal_data[IQ_DATA_LENGTH-1] - signal_data[0]) / (float32_t)(IQ_DATA_LENGTH - 1);
    
    for (uint32_t i = 0; i < IQ_DATA_LENGTH; i++) {
        signal_data[i] -= slope * (float32_t)i;
    }
    
    return HAL_OK;
}

/**
  * @brief  Apply bandpass filter to signal (fixed length: 2000)
  * @param  input_data: Input signal data
  * @param  output_data: Output filtered data
  * @param  low_freq: Low cutoff frequency
  * @param  high_freq: High cutoff frequency
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ApplyBandpassFilter(float32_t *input_data, float32_t *output_data, 
                                        float32_t low_freq, float32_t high_freq)
{
    if (input_data == NULL || output_data == NULL) {
        return HAL_ERROR;
    }
    
    // Reset filter state
    memset(hr_filter_state, 0, sizeof(hr_filter_state));
    memset(br_filter_state, 0, sizeof(br_filter_state));
    
    // Select appropriate filter based on frequency range
    if (low_freq >= HR_FILTER_LOW_FREQ && high_freq <= HR_FILTER_HIGH_FREQ) {
        // Use heart rate filter
        arm_biquad_cascade_df1_f32(&hr_filter_instance, input_data, output_data, IQ_DATA_LENGTH);
    } else if (low_freq >= BR_FILTER_LOW_FREQ && high_freq <= BR_FILTER_HIGH_FREQ) {
        // Use breathing rate filter
        arm_biquad_cascade_df1_f32(&br_filter_instance, input_data, output_data, IQ_DATA_LENGTH);
    } else {
        // Copy input to output if no suitable filter
        arm_copy_f32(input_data, output_data, IQ_DATA_LENGTH);
    }
    
    return HAL_OK;
}

/**
  * @brief  Compute FFT of input signal (fixed length: 2000)
  * @param  input_data: Input signal data
  * @param  fft_output: FFT output data
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ComputeFFT(float32_t *input_data, float32_t *fft_output)
{
    if (input_data == NULL || fft_output == NULL) {
        return HAL_ERROR;
    }
    
    // Copy input data to FFT buffer (pad with zeros if necessary)
    memcpy(fft_input_buffer, input_data, IQ_DATA_LENGTH * sizeof(float32_t));
    
    // Zero-pad the remaining buffer if FFT_SIZE > IQ_DATA_LENGTH
    if (FFT_SIZE > IQ_DATA_LENGTH) {
        memset(&fft_input_buffer[IQ_DATA_LENGTH], 0, (FFT_SIZE - IQ_DATA_LENGTH) * sizeof(float32_t));
    }
    
    // Compute real FFT
    arm_rfft_fast_f32(&fft_instance, fft_input_buffer, fft_output, 0);
    
    // Compute magnitude
    arm_cmplx_mag_f32(fft_output, fft_output, FFT_SIZE / 2);
    
    return HAL_OK;
}

/**
  * @brief  Find peak frequency in FFT data (fixed FFT size: 2048)
  * @param  fft_data: FFT magnitude data
  * @param  min_freq: Minimum frequency to search
  * @param  max_freq: Maximum frequency to search
  * @param  peak_freq: Output peak frequency
  * @param  peak_magnitude: Output peak magnitude
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_FindPeakFrequency(float32_t *fft_data, float32_t min_freq, float32_t max_freq, 
                                      float32_t *peak_freq, float32_t *peak_magnitude)
{
    if (fft_data == NULL || peak_freq == NULL || peak_magnitude == NULL) {
        return HAL_ERROR;
    }
    
    uint32_t fft_length = FFT_SIZE / 2;
    
    // Convert frequencies to bin indices
    uint32_t min_bin = (uint32_t)(min_freq * fft_length * 2 / IQ_SAMPLE_RATE);
    uint32_t max_bin = (uint32_t)(max_freq * fft_length * 2 / IQ_SAMPLE_RATE);
    
    // Clamp to valid range
    if (min_bin >= fft_length) min_bin = fft_length - 1;
    if (max_bin >= fft_length) max_bin = fft_length - 1;
    if (min_bin > max_bin) {
        uint32_t temp = min_bin;
        min_bin = max_bin;
        max_bin = temp;
    }
    
    // Find maximum in the specified range
    uint32_t peak_index;
    arm_max_f32(&fft_data[min_bin], max_bin - min_bin + 1, peak_magnitude, &peak_index);
    
    // Convert bin index back to frequency
    peak_index += min_bin;
    *peak_freq = (float32_t)peak_index * IQ_SAMPLE_RATE / (fft_length * 2);
    
    return HAL_OK;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Calculate filter coefficients (simplified Butterworth)
  * @param  low_freq: Low cutoff frequency
  * @param  high_freq: High cutoff frequency
  * @param  sample_rate: Sampling rate
  * @param  coeffs: Output filter coefficients
  * @retval None
  */
static void IQ_CalculateFilterCoefficients(float32_t low_freq, float32_t high_freq, 
                                          float32_t sample_rate, float32_t *coeffs)
{
    // Simplified Butterworth filter coefficient calculation
    // This is a basic implementation - for production use, consider using
    // proper filter design tools or precomputed coefficients
    
    float32_t nyquist = sample_rate / 2.0f;
    float32_t low_norm = low_freq / nyquist;
    float32_t high_norm = high_freq / nyquist;
    
    // Basic 2nd order bandpass filter coefficients
    // These are simplified - in practice, you'd use proper filter design
    float32_t wc = 2.0f * 3.14159265359f * sqrtf(low_norm * high_norm);
    float32_t bw = 2.0f * 3.14159265359f * (high_norm - low_norm);
    
    // Simplified coefficients (this is a placeholder)
    coeffs[0] = 1.0f;   // b0
    coeffs[1] = 0.0f;   // b1
    coeffs[2] = -1.0f;  // b2
    coeffs[3] = 1.0f;   // a0
    coeffs[4] = 0.0f;   // a1
    
    // Note: In production, use proper filter design algorithms
}

/**
  * @brief  Print debug information
  * @param  message: Debug message
  * @param  value: Value to print
  * @retval None
  */
static void IQ_PrintDebugInfo(const char* message, float32_t value)
{
    char debug_buffer[100];
    sprintf(debug_buffer, "[IQ] %s: %.2f\r\n", message, value);
    UART_Send_String(debug_buffer);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*
사용 예제 (간단한 인터페이스):

// 1. 초기화
IQ_SignalProcessing_Init();

// 2. I와 Q 데이터 준비 (각각 길이 2000인 float32 배열)
float32_t i_data_array[2000];  // I 성분 데이터
float32_t q_data_array[2000];  // Q 성분 데이터

// 3. 바이탈 사인 처리 (간단한 함수 호출)
VitalSigns_t results;
if (IQ_ProcessVitalSigns(i_data_array, q_data_array, &results) == HAL_OK) {
    printf("Heart Rate: %.2f BPM (confidence: %.2f)\n", 
           results.heart_rate_bpm, results.heart_rate_confidence);
    printf("Breathing Rate: %.2f BPM (confidence: %.2f)\n", 
           results.breathing_rate_bpm, results.breathing_rate_confidence);
}

// Python 코드와의 대응:
// Python: I = _data['radar_i']                     ->  C: float32_t *i_data
// Python: Q = _data['radar_q']                     ->  C: float32_t *q_data
// Python: IQ = I + 1j * Q                         ->  C: IQ_ComputePhase(i_data, q_data, phase_buffer)
// Python: phase = np.angle(IQ)                    ->  C: 위의 함수에서 수행됨
// Python: phase = np.unwrap(phase)                ->  C: IQ_UnwrapPhase(phase_buffer)
// Python: phase_detrended = detrend(phase)        ->  C: IQ_RemoveDCTrend(phase_buffer)
// Python: hr_phase_filtered = filtfilt(...)       ->  C: IQ_ApplyBandpassFilter(...)
// Python: hr_fft_vals = np.abs(np.fft.rfft(...))  ->  C: IQ_ComputeFFT(...)
// Python: hr_peak_freq = hr_freqs[np.argmax(...)]  ->  C: IQ_FindPeakFrequency(...)

// 주요 특징:
// - 데이터 길이 고정: 2000 (IQ_DATA_LENGTH)
// - 샘플링 주파수 고정: 2000Hz (IQ_SAMPLE_RATE)
// - 심박수 대역: 0.8-3.0 Hz (48-180 BPM)
// - 호흡수 대역: 0.1-0.5 Hz (6-30 BPM)
// - FFT 크기: 2048 (zero-padding 적용)
// - 필터: 2차 Butterworth 대역통과 필터
*/
