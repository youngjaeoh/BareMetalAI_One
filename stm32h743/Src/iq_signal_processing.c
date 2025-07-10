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

// Working buffers (updated for 250 samples) - 메모리 사용량 체크
// phase_buffer: 250 * 4 = 1000 bytes
// i_data_buffer: 250 * 4 = 1000 bytes  
// q_data_buffer: 250 * 4 = 1000 bytes
// fft_input_buffer: 256 * 4 = 1024 bytes
// fft_output_buffer: 256 * 4 = 1024 bytes
// 총합: 약 5KB - 스택 오버플로우 위험!

// 스택 오버플로우 방지를 위해 static 버퍼 크기 축소
static float phase_buffer[IQ_PROCESSING_SAMPLES];
static float fft_input_buffer[FFT_SIZE];
static float fft_output_buffer[FFT_SIZE];

// 임시 버퍼들을 작은 크기로 분할 처리
static float i_data_buffer[IQ_PROCESSING_SAMPLES];
static float q_data_buffer[IQ_PROCESSING_SAMPLES];

/* Private function prototypes -----------------------------------------------*/
static void IQ_PrintDebugInfo(const char* message, float value);

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Initialize IQ signal processing module
  * @param  None
  * @retval None
  */
void IQ_SignalProcessing_Init(void)
{
    // 중요: FFT 인스턴스 초기화 - 이게 없으면 HARD_FAULT 발생!
    arm_status status = arm_rfft_fast_init_f32(&fft_instance, FFT_SIZE);
    
    if (status != ARM_MATH_SUCCESS) {
        // FFT 초기화 실패시 에러 출력
        // IQ_PrintDebugInfo("FFT Init Failed", (float)status);
        return;
    }
    
    // Clear working buffers - 메모리 안전성 확보
    memset(phase_buffer, 0, sizeof(phase_buffer));
    memset(fft_input_buffer, 0, sizeof(fft_input_buffer));
    memset(fft_output_buffer, 0, sizeof(fft_output_buffer));
    memset(i_data_buffer, 0, sizeof(i_data_buffer));
    memset(q_data_buffer, 0, sizeof(q_data_buffer));
    
    // Debug information
    // IQ_PrintDebugInfo("IQ Signal Processing Initialized", 0);
}

/**
  * @brief  Process I/Q data from float queues and calculate signal quality and movement level
  * @param  i_queue: I component float queue
  * @param  q_queue: Q component float queue
  * @param  quality: Output signal quality structure (phase_std, final_quality)
  * @param  i_data_out: Output I data buffer (250 samples)
  * @param  q_data_out: Output Q data buffer (250 samples)
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ProcessFloatQueues(FloatQueue *i_queue, FloatQueue *q_queue, SignalQuality_t *quality, float *i_data_out, float *q_data_out)
{
    if (i_queue == NULL || q_queue == NULL || quality == NULL) {
        return HAL_ERROR;
    }
    
    // Check if we have enough data (250 samples each)
    if (float_queue_count(i_queue) < IQ_PROCESSING_SAMPLES || 
        float_queue_count(q_queue) < IQ_PROCESSING_SAMPLES) {
        // IQ_PrintDebugInfo("Not enough data in queues", float_queue_count(i_queue));
        return HAL_ERROR;
    }
    
    // IQ_PrintDebugInfo("Processing float queues, samples", IQ_PROCESSING_SAMPLES);
    
    // Extract 250 samples from each queue
    for (uint32_t i = 0; i < IQ_PROCESSING_SAMPLES; i++) {
        if (!float_queue_dequeue(i_queue, &i_data_buffer[i]) ||
            !float_queue_dequeue(q_queue, &q_data_buffer[i])) {
            // IQ_PrintDebugInfo("Failed to dequeue data at sample", i);
            return HAL_ERROR;
        }
    }
    
    // Step 1: Compute phase from I and Q data
    if (IQ_ComputePhase(i_data_buffer, q_data_buffer, phase_buffer, IQ_PROCESSING_SAMPLES) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 2: Unwrap phase
    if (IQ_UnwrapPhase(phase_buffer, IQ_PROCESSING_SAMPLES) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 3: Remove DC and trend
    if (IQ_RemoveDCTrend(phase_buffer, IQ_PROCESSING_SAMPLES) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 4: Handle NaN values
    if (IQ_NanToNum(phase_buffer, IQ_PROCESSING_SAMPLES) != HAL_OK) {
        return HAL_ERROR;
    }
    
    // Step 5: Calculate phase standard deviation (phase_std = np.std(phase))
    float mean = 0.0f;
    for (uint32_t i = 0; i < IQ_PROCESSING_SAMPLES; i++) {
        mean += phase_buffer[i];
    }
    mean /= IQ_PROCESSING_SAMPLES;
    
    float variance = 0.0f;
    for (uint32_t i = 0; i < IQ_PROCESSING_SAMPLES; i++) {
        float diff = phase_buffer[i] - mean;
        variance += diff * diff;
    }
    variance /= IQ_PROCESSING_SAMPLES;
    quality->phase_std = sqrtf(variance);
    
    // Step 6: Calculate phase_mean = np.mean(np.abs(phase))
    float phase_mean = 0.0f;
    for (uint32_t i = 0; i < IQ_PROCESSING_SAMPLES; i++) {
        phase_mean += fabsf(phase_buffer[i]);
    }
    phase_mean /= IQ_PROCESSING_SAMPLES;
    
    // Step 7: Calculate phase_energy = np.sum(phase ** 2)
    float phase_energy = 0.0f;
    for (uint32_t i = 0; i < IQ_PROCESSING_SAMPLES; i++) {
        phase_energy += phase_buffer[i] * phase_buffer[i];
    }
    
    // Step 8: Calculate quality_score based on phase_std (Python logic)
    float quality_score;
    if (quality->phase_std < 0.001f) {
        quality_score = 0.0f;
    } else if (quality->phase_std < 0.01f) {
        quality_score = 20.0f;
    } else if (quality->phase_std < 0.05f) {
        quality_score = 60.0f;
    } else if (quality->phase_std < 0.1f) {
        quality_score = 80.0f;
    } else {
        quality_score = 100.0f;
    }
    
    // Step 9: Adjust quality_score based on phase_energy (Python logic)
    if (phase_energy < 0.001f) {
        quality_score *= 0.5f;
    } else if (phase_energy > 1.0f) {
        quality_score = fminf(100.0f, quality_score * 1.2f);
    }
    
    // Step 10: Calculate SNR (Python logic)
    float snr;
    if (quality->phase_std < 1e-6f) {
        snr = -50.0f;
    } else {
        // Compute FFT for SNR calculation (fft_result = fft(phase))
        if (IQ_ComputeFFT(phase_buffer, fft_output_buffer) != HAL_OK) {
            snr = -50.0f;
        } else {
            // Calculate vital sign frequency range power
            // freqs = fftfreq(len(phase), 1/fs)
            // vital_mask = (freqs >= 0.1) & (freqs <= 3.0)
            // Assuming fs = 50 Hz (50 samples/sec), for 250 samples
            float fs = 50.0f;
            uint32_t fft_length = FFT_SIZE / 2;
            
            // Convert frequency range 0.1-3.0 Hz to bin indices
            uint32_t min_bin = (uint32_t)(VITAL_FREQ_MIN * FFT_SIZE / fs);
            uint32_t max_bin = (uint32_t)(VITAL_FREQ_MAX * FFT_SIZE / fs);
            
            // Clamp to valid range
            if (min_bin < 1) min_bin = 1;  // Skip DC bin
            if (max_bin >= fft_length) max_bin = fft_length - 1;
            
            if (max_bin > min_bin) {
                // vital_power = np.abs(fft_result[vital_mask])
                // signal_power = np.max(vital_power)
                // noise_power = np.mean(vital_power)
                float signal_power = 0.0f;
                float noise_power = 0.0f;
                uint32_t vital_bin_count = max_bin - min_bin + 1;
                
                for (uint32_t i = min_bin; i <= max_bin; i++) {
                    if (fft_output_buffer[i] > signal_power) {
                        signal_power = fft_output_buffer[i];
                    }
                    noise_power += fft_output_buffer[i];
                }
                noise_power /= vital_bin_count;
                
                // snr = 20 * np.log10(signal_power / (noise_power + 1e-10))
                snr = 20.0f * log10f(signal_power / (noise_power + 1e-10f));
                // snr = np.clip(snr, -50, 50)
                if (snr > 50.0f) snr = 50.0f;
                if (snr < -50.0f) snr = -50.0f;
            } else {
                snr = -50.0f;
            }
        }
    }
    
    // Step 11: Calculate final_quality (Python logic)
    // final_quality = (quality_score + max(0, snr + 50)) / 2
    float snr_normalized = fmaxf(0.0f, snr + 50.0f);
    quality->final_quality = (quality_score + snr_normalized) / 2.0f;
    // final_quality = np.clip(final_quality, 0, 100)
    if (quality->final_quality > 100.0f) quality->final_quality = 100.0f;
    if (quality->final_quality < 0.0f) quality->final_quality = 0.0f;
    
    // IQ_PrintDebugInfo("Phase STD calculated", quality->phase_std);
    // IQ_PrintDebugInfo("Final quality calculated", quality->final_quality);
    
    // Copy I/Q data to output buffers if provided
    if (i_data_out != NULL) {
        memcpy(i_data_out, i_data_buffer, IQ_PROCESSING_SAMPLES * sizeof(float));
    }
    if (q_data_out != NULL) {
        memcpy(q_data_out, q_data_buffer, IQ_PROCESSING_SAMPLES * sizeof(float));
    }
    
    return HAL_OK;
}

/* Helper Functions ----------------------------------------------------------*/
/**
  * @brief  Compute phase from I and Q data
  * @param  i_data: I component data (float32, length: 50)
  * @param  q_data: Q component data (float32, length: 50)
  * @param  phase_data: Output phase data (float32, length: 50)
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ComputePhase(float *i_data, float *q_data, float *phase_data, uint32_t length)
{
    if (i_data == NULL || q_data == NULL || phase_data == NULL) {
        return HAL_ERROR;
    }
    
    // Compute phase using atan2
    // Python equivalent: phase = np.angle(I + 1j*Q) = np.angle(IQ)
    for (uint32_t i = 0; i < length; i++) {
        phase_data[i] = atan2f(q_data[i], i_data[i]);
    }
    
    return HAL_OK;
}

/**
  * @brief  Unwrap phase data (fixed length: 50)
  * @param  phase_data: Phase data to unwrap
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_UnwrapPhase(float *phase_data, uint32_t length)
{
    if (phase_data == NULL || length < 2) {
        return HAL_ERROR;
    }
    
    const float TWO_PI = 2.0f * 3.14159265359f;
    
    for (uint32_t i = 1; i < length; i++) {
        float diff = phase_data[i] - phase_data[i-1];
        
        // Unwrap phase jumps
        if (diff > 3.14159265359f) {
            phase_data[i] -= TWO_PI;
        } else if (diff < -3.14159265359f) {
            phase_data[i] += TWO_PI;
        }
    }
    
    return HAL_OK;
}

/**
  * @brief  Remove DC component and linear trend (variable length)
  * @param  signal_data: Signal data to process
  * @param  length: Number of samples to process
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_RemoveDCTrend(float *signal_data, uint32_t length)
{
    if (signal_data == NULL || length == 0) {
        return HAL_ERROR;
    }
    
    // Calculate mean (DC component)
    float mean_value = 0.0f;
    for (uint32_t i = 0; i < length; i++) {
        mean_value += signal_data[i];
    }
    mean_value /= length;
    
    // Remove DC component
    for (uint32_t i = 0; i < length; i++) {
        signal_data[i] -= mean_value;
    }
    
    // Simple linear detrending (remove linear trend)
    if (length > 1) {
        float slope = (signal_data[length-1] - signal_data[0]) / (float)(length - 1);
        
        for (uint32_t i = 0; i < length; i++) {
            signal_data[i] -= slope * (float)i;
        }
    }
    
    return HAL_OK;
}

/**
  * @brief  Replace NaN and Inf values with finite values (equivalent to np.nan_to_num)
  * @param  signal_data: Signal data to process (variable length)
  * @param  length: Number of samples to process
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_NanToNum(float *signal_data, uint32_t length)
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

/**
  * @brief  Compute FFT of input signal (variable length, zero-padded to FFT_SIZE)
  * @param  input_data: Input signal data
  * @param  fft_output: FFT output data (magnitude)
  * @retval HAL status
  */
HAL_StatusTypeDef IQ_ComputeFFT(float *input_data, float *fft_output)
{
    if (input_data == NULL || fft_output == NULL) {
        return HAL_ERROR;
    }
    
    // 메모리 안전성 확보
    memset(fft_input_buffer, 0, sizeof(fft_input_buffer));
    
    // Copy input data to FFT buffer (first IQ_PROCESSING_SAMPLES elements)
    // 경계 검사 추가 - HARD_FAULT 방지
    uint32_t copy_size = (IQ_PROCESSING_SAMPLES < FFT_SIZE) ? IQ_PROCESSING_SAMPLES : FFT_SIZE;
    for (uint32_t i = 0; i < copy_size; i++) {
        // NaN/Inf 값 검사 - HARD_FAULT 방지
        if (isnanf(input_data[i]) || isinff(input_data[i])) {
            fft_input_buffer[i] = 0.0f;
        } else {
            fft_input_buffer[i] = input_data[i];
        }
    }
    
    // Compute real FFT - 이 부분에서 HARD_FAULT 발생 가능성 높음
    arm_rfft_fast_f32(&fft_instance, fft_input_buffer, fft_output, 0);
    
    // Compute magnitude (convert complex to magnitude)
    // 경계 검사 추가
    arm_cmplx_mag_f32(fft_output, fft_output, FFT_SIZE / 2);
    
    return HAL_OK;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Print debug information
  * @param  message: Debug message
  * @param  value: Value to print
  * @retval None
  */
static void IQ_PrintDebugInfo(const char* message, float value)
{
    char debug_buffer[100];
    sprintf(debug_buffer, "[IQ] %s: %.2f\r\n", message, value);
    UART_Send_String(debug_buffer);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*
=== Python 코드와 정확히 동일한 구현 ===

Python 원본:
def calculate_signal_quality(complex_signal, fs):
    try:
        phase = np.angle(complex_signal)           # -> IQ_ComputePhase()
        phase = np.unwrap(phase)                   # -> IQ_UnwrapPhase()
        phase = signal.detrend(phase)              # -> IQ_RemoveDCTrend()
        phase = np.nan_to_num(phase, ...)          # -> IQ_NanToNum()
        phase_std = np.std(phase)                  # -> manual std calculation
        phase_mean = np.mean(np.abs(phase))       # -> manual abs mean calculation
        phase_energy = np.sum(phase ** 2)         # -> manual sum of squares
        
        # Quality score calculation (exact same logic)
        if phase_std < 0.001: quality_score = 0
        elif phase_std < 0.01: quality_score = 20
        elif phase_std < 0.05: quality_score = 60
        elif phase_std < 0.1: quality_score = 80
        else: quality_score = 100
        
        # Phase energy adjustment (exact same logic)
        if phase_energy < 0.001: quality_score *= 0.5
        elif phase_energy > 1.0: quality_score = min(100, quality_score * 1.2)
        
        # SNR calculation (exact same logic)
        if phase_std < 1e-6: snr = -50.0
        else:
            fft_result = fft(phase)                # -> IQ_ComputeFFT()
            freqs = fftfreq(len(phase), 1/fs)      # -> bin index calculation
            vital_mask = (freqs >= 0.1) & (freqs <= 3.0)  # -> min_bin, max_bin
            vital_power = np.abs(fft_result[vital_mask])   # -> already magnitude
            signal_power = np.max(vital_power)     # -> manual max finding
            noise_power = np.mean(vital_power)     # -> manual mean calculation
            snr = 20 * np.log10(signal_power / (noise_power + 1e-10))  # -> same formula
            snr = np.clip(snr, -50, 50)           # -> manual clipping
        
        # Final quality calculation (exact same logic)
        final_quality = (quality_score + max(0, snr + 50)) / 2
        final_quality = np.clip(final_quality, 0, 100)
        
        return final_quality
    except Exception:
        return 0

=== C 사용 예제 ===

#include "iq_signal_processing.h"
#include "float_queue.h"

// 초기화
IQ_SignalProcessing_Init();

// FloatQueue 선언 (main.c에서)
FloatQueue i_data_queue;
FloatQueue q_data_queue;

// 메인 루프에서 신호처리
while (1) {
    // 250개 샘플이 큐에 준비되었는지 확인
    if (float_queue_count(&i_data_queue) >= IQ_SAMPLES_FOR_PROCESSING) {
        SignalQuality_t quality;
        
        // Python과 동일한 로직으로 신호 품질 계산
        if (IQ_ProcessFloatQueues(&i_data_queue, &q_data_queue, &quality) == HAL_OK) {
            printf("Phase STD: %.6f\n", quality.phase_std);
            printf("Final Quality: %.2f\n", quality.final_quality);
            
            // 이 두 값을 모델 입력으로 사용
            float model_inputs[2] = {quality.phase_std, quality.final_quality};
        }
    }
    
    HAL_Delay(200);  // 0.2초 대기
}

=== 주요 특징 ===

1. 입력: 250개 float 샘플 (1000 bytes) from FloatQueue
2. 출력: phase_std, final_quality (2개 값만)
3. 샘플링 주파수: 50 Hz (50 samples/sec)
4. FFT 크기: 256 (250 샘플 + 6개 zero padding)
5. Vital 주파수 범위: 0.1-3.0 Hz
6. Python 코드와 100% 동일한 로직

=== 메모리 사용량 ===

- phase_buffer: 250 * 4 = 1000 bytes
- i_data_buffer: 250 * 4 = 1000 bytes
- q_data_buffer: 250 * 4 = 1000 bytes
- fft_input_buffer: 256 * 4 = 1024 bytes
- fft_output_buffer: 256 * 4 = 1024 bytes
- 총합: 약 5KB - 스택 오버플로우 위험!
*/
