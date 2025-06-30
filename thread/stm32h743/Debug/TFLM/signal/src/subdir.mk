################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/signal/src/circular_buffer.cc \
../TFLM/signal/src/energy.cc \
../TFLM/signal/src/fft_auto_scale.cc \
../TFLM/signal/src/filter_bank.cc \
../TFLM/signal/src/filter_bank_log.cc \
../TFLM/signal/src/filter_bank_spectral_subtraction.cc \
../TFLM/signal/src/filter_bank_square_root.cc \
../TFLM/signal/src/irfft_float.cc \
../TFLM/signal/src/irfft_int16.cc \
../TFLM/signal/src/irfft_int32.cc \
../TFLM/signal/src/log.cc \
../TFLM/signal/src/max_abs.cc \
../TFLM/signal/src/msb_32.cc \
../TFLM/signal/src/msb_64.cc \
../TFLM/signal/src/overlap_add.cc \
../TFLM/signal/src/pcan_argc_fixed.cc \
../TFLM/signal/src/rfft_float.cc \
../TFLM/signal/src/rfft_int16.cc \
../TFLM/signal/src/rfft_int32.cc \
../TFLM/signal/src/square_root_32.cc \
../TFLM/signal/src/square_root_64.cc \
../TFLM/signal/src/window.cc 

CC_DEPS += \
./TFLM/signal/src/circular_buffer.d \
./TFLM/signal/src/energy.d \
./TFLM/signal/src/fft_auto_scale.d \
./TFLM/signal/src/filter_bank.d \
./TFLM/signal/src/filter_bank_log.d \
./TFLM/signal/src/filter_bank_spectral_subtraction.d \
./TFLM/signal/src/filter_bank_square_root.d \
./TFLM/signal/src/irfft_float.d \
./TFLM/signal/src/irfft_int16.d \
./TFLM/signal/src/irfft_int32.d \
./TFLM/signal/src/log.d \
./TFLM/signal/src/max_abs.d \
./TFLM/signal/src/msb_32.d \
./TFLM/signal/src/msb_64.d \
./TFLM/signal/src/overlap_add.d \
./TFLM/signal/src/pcan_argc_fixed.d \
./TFLM/signal/src/rfft_float.d \
./TFLM/signal/src/rfft_int16.d \
./TFLM/signal/src/rfft_int32.d \
./TFLM/signal/src/square_root_32.d \
./TFLM/signal/src/square_root_64.d \
./TFLM/signal/src/window.d 

OBJS += \
./TFLM/signal/src/circular_buffer.o \
./TFLM/signal/src/energy.o \
./TFLM/signal/src/fft_auto_scale.o \
./TFLM/signal/src/filter_bank.o \
./TFLM/signal/src/filter_bank_log.o \
./TFLM/signal/src/filter_bank_spectral_subtraction.o \
./TFLM/signal/src/filter_bank_square_root.o \
./TFLM/signal/src/irfft_float.o \
./TFLM/signal/src/irfft_int16.o \
./TFLM/signal/src/irfft_int32.o \
./TFLM/signal/src/log.o \
./TFLM/signal/src/max_abs.o \
./TFLM/signal/src/msb_32.o \
./TFLM/signal/src/msb_64.o \
./TFLM/signal/src/overlap_add.o \
./TFLM/signal/src/pcan_argc_fixed.o \
./TFLM/signal/src/rfft_float.o \
./TFLM/signal/src/rfft_int16.o \
./TFLM/signal/src/rfft_int32.o \
./TFLM/signal/src/square_root_32.o \
./TFLM/signal/src/square_root_64.o \
./TFLM/signal/src/window.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/signal/src/%.o TFLM/signal/src/%.su TFLM/signal/src/%.cyclo: ../TFLM/signal/src/%.cc TFLM/signal/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-signal-2f-src

clean-TFLM-2f-signal-2f-src:
	-$(RM) ./TFLM/signal/src/circular_buffer.cyclo ./TFLM/signal/src/circular_buffer.d ./TFLM/signal/src/circular_buffer.o ./TFLM/signal/src/circular_buffer.su ./TFLM/signal/src/energy.cyclo ./TFLM/signal/src/energy.d ./TFLM/signal/src/energy.o ./TFLM/signal/src/energy.su ./TFLM/signal/src/fft_auto_scale.cyclo ./TFLM/signal/src/fft_auto_scale.d ./TFLM/signal/src/fft_auto_scale.o ./TFLM/signal/src/fft_auto_scale.su ./TFLM/signal/src/filter_bank.cyclo ./TFLM/signal/src/filter_bank.d ./TFLM/signal/src/filter_bank.o ./TFLM/signal/src/filter_bank.su ./TFLM/signal/src/filter_bank_log.cyclo ./TFLM/signal/src/filter_bank_log.d ./TFLM/signal/src/filter_bank_log.o ./TFLM/signal/src/filter_bank_log.su ./TFLM/signal/src/filter_bank_spectral_subtraction.cyclo ./TFLM/signal/src/filter_bank_spectral_subtraction.d ./TFLM/signal/src/filter_bank_spectral_subtraction.o ./TFLM/signal/src/filter_bank_spectral_subtraction.su ./TFLM/signal/src/filter_bank_square_root.cyclo ./TFLM/signal/src/filter_bank_square_root.d ./TFLM/signal/src/filter_bank_square_root.o ./TFLM/signal/src/filter_bank_square_root.su ./TFLM/signal/src/irfft_float.cyclo ./TFLM/signal/src/irfft_float.d ./TFLM/signal/src/irfft_float.o ./TFLM/signal/src/irfft_float.su ./TFLM/signal/src/irfft_int16.cyclo ./TFLM/signal/src/irfft_int16.d ./TFLM/signal/src/irfft_int16.o ./TFLM/signal/src/irfft_int16.su ./TFLM/signal/src/irfft_int32.cyclo ./TFLM/signal/src/irfft_int32.d ./TFLM/signal/src/irfft_int32.o ./TFLM/signal/src/irfft_int32.su ./TFLM/signal/src/log.cyclo ./TFLM/signal/src/log.d ./TFLM/signal/src/log.o ./TFLM/signal/src/log.su ./TFLM/signal/src/max_abs.cyclo ./TFLM/signal/src/max_abs.d ./TFLM/signal/src/max_abs.o ./TFLM/signal/src/max_abs.su ./TFLM/signal/src/msb_32.cyclo ./TFLM/signal/src/msb_32.d ./TFLM/signal/src/msb_32.o ./TFLM/signal/src/msb_32.su ./TFLM/signal/src/msb_64.cyclo ./TFLM/signal/src/msb_64.d ./TFLM/signal/src/msb_64.o ./TFLM/signal/src/msb_64.su ./TFLM/signal/src/overlap_add.cyclo ./TFLM/signal/src/overlap_add.d ./TFLM/signal/src/overlap_add.o ./TFLM/signal/src/overlap_add.su ./TFLM/signal/src/pcan_argc_fixed.cyclo ./TFLM/signal/src/pcan_argc_fixed.d ./TFLM/signal/src/pcan_argc_fixed.o ./TFLM/signal/src/pcan_argc_fixed.su ./TFLM/signal/src/rfft_float.cyclo ./TFLM/signal/src/rfft_float.d ./TFLM/signal/src/rfft_float.o ./TFLM/signal/src/rfft_float.su ./TFLM/signal/src/rfft_int16.cyclo ./TFLM/signal/src/rfft_int16.d ./TFLM/signal/src/rfft_int16.o ./TFLM/signal/src/rfft_int16.su ./TFLM/signal/src/rfft_int32.cyclo ./TFLM/signal/src/rfft_int32.d ./TFLM/signal/src/rfft_int32.o ./TFLM/signal/src/rfft_int32.su ./TFLM/signal/src/square_root_32.cyclo ./TFLM/signal/src/square_root_32.d ./TFLM/signal/src/square_root_32.o ./TFLM/signal/src/square_root_32.su ./TFLM/signal/src/square_root_64.cyclo ./TFLM/signal/src/square_root_64.d ./TFLM/signal/src/square_root_64.o ./TFLM/signal/src/square_root_64.su ./TFLM/signal/src/window.cyclo ./TFLM/signal/src/window.d ./TFLM/signal/src/window.o ./TFLM/signal/src/window.su

.PHONY: clean-TFLM-2f-signal-2f-src

