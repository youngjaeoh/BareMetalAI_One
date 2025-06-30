################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.c \
../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.c 

C_DEPS += \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.d \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.d 

OBJS += \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.o \
./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/%.o TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/%.su TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/%.cyclo: ../TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/%.c TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DTFT96 -D__USE_CMSIS -DCMSIS_NN -DCMSIS_OPTIMIZATION_LEVEL -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -lnosys -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-NNSupportFunctions

clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-NNSupportFunctions:
	-$(RM) ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s4.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_calculate_gate_s8_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_lstm_step_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s4.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_interleaved_t_even_s4.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s4.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8_s32.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_transpose_conv_row_s8_s32.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.cyclo
	-$(RM) ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mul_result_acc_s8_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_per_ch_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16_s16.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s4.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_nntables.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_q7_to_q15_with_offset.su ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.cyclo ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.d ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.o ./TFLM/third_party/cmsis_nn/Source/NNSupportFunctions/arm_s8_to_s16_unordered_with_offset.su

.PHONY: clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-NNSupportFunctions

