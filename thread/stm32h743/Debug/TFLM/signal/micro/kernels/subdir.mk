################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/signal/micro/kernels/delay.cc \
../TFLM/signal/micro/kernels/energy.cc \
../TFLM/signal/micro/kernels/fft_auto_scale_common.cc \
../TFLM/signal/micro/kernels/fft_auto_scale_kernel.cc \
../TFLM/signal/micro/kernels/filter_bank.cc \
../TFLM/signal/micro/kernels/filter_bank_log.cc \
../TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.cc \
../TFLM/signal/micro/kernels/filter_bank_square_root.cc \
../TFLM/signal/micro/kernels/filter_bank_square_root_common.cc \
../TFLM/signal/micro/kernels/framer.cc \
../TFLM/signal/micro/kernels/irfft.cc \
../TFLM/signal/micro/kernels/overlap_add.cc \
../TFLM/signal/micro/kernels/pcan.cc \
../TFLM/signal/micro/kernels/rfft.cc \
../TFLM/signal/micro/kernels/stacker.cc \
../TFLM/signal/micro/kernels/window.cc 

CC_DEPS += \
./TFLM/signal/micro/kernels/delay.d \
./TFLM/signal/micro/kernels/energy.d \
./TFLM/signal/micro/kernels/fft_auto_scale_common.d \
./TFLM/signal/micro/kernels/fft_auto_scale_kernel.d \
./TFLM/signal/micro/kernels/filter_bank.d \
./TFLM/signal/micro/kernels/filter_bank_log.d \
./TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.d \
./TFLM/signal/micro/kernels/filter_bank_square_root.d \
./TFLM/signal/micro/kernels/filter_bank_square_root_common.d \
./TFLM/signal/micro/kernels/framer.d \
./TFLM/signal/micro/kernels/irfft.d \
./TFLM/signal/micro/kernels/overlap_add.d \
./TFLM/signal/micro/kernels/pcan.d \
./TFLM/signal/micro/kernels/rfft.d \
./TFLM/signal/micro/kernels/stacker.d \
./TFLM/signal/micro/kernels/window.d 

OBJS += \
./TFLM/signal/micro/kernels/delay.o \
./TFLM/signal/micro/kernels/energy.o \
./TFLM/signal/micro/kernels/fft_auto_scale_common.o \
./TFLM/signal/micro/kernels/fft_auto_scale_kernel.o \
./TFLM/signal/micro/kernels/filter_bank.o \
./TFLM/signal/micro/kernels/filter_bank_log.o \
./TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.o \
./TFLM/signal/micro/kernels/filter_bank_square_root.o \
./TFLM/signal/micro/kernels/filter_bank_square_root_common.o \
./TFLM/signal/micro/kernels/framer.o \
./TFLM/signal/micro/kernels/irfft.o \
./TFLM/signal/micro/kernels/overlap_add.o \
./TFLM/signal/micro/kernels/pcan.o \
./TFLM/signal/micro/kernels/rfft.o \
./TFLM/signal/micro/kernels/stacker.o \
./TFLM/signal/micro/kernels/window.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/signal/micro/kernels/%.o TFLM/signal/micro/kernels/%.su TFLM/signal/micro/kernels/%.cyclo: ../TFLM/signal/micro/kernels/%.cc TFLM/signal/micro/kernels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-signal-2f-micro-2f-kernels

clean-TFLM-2f-signal-2f-micro-2f-kernels:
	-$(RM) ./TFLM/signal/micro/kernels/delay.cyclo ./TFLM/signal/micro/kernels/delay.d ./TFLM/signal/micro/kernels/delay.o ./TFLM/signal/micro/kernels/delay.su ./TFLM/signal/micro/kernels/energy.cyclo ./TFLM/signal/micro/kernels/energy.d ./TFLM/signal/micro/kernels/energy.o ./TFLM/signal/micro/kernels/energy.su ./TFLM/signal/micro/kernels/fft_auto_scale_common.cyclo ./TFLM/signal/micro/kernels/fft_auto_scale_common.d ./TFLM/signal/micro/kernels/fft_auto_scale_common.o ./TFLM/signal/micro/kernels/fft_auto_scale_common.su ./TFLM/signal/micro/kernels/fft_auto_scale_kernel.cyclo ./TFLM/signal/micro/kernels/fft_auto_scale_kernel.d ./TFLM/signal/micro/kernels/fft_auto_scale_kernel.o ./TFLM/signal/micro/kernels/fft_auto_scale_kernel.su ./TFLM/signal/micro/kernels/filter_bank.cyclo ./TFLM/signal/micro/kernels/filter_bank.d ./TFLM/signal/micro/kernels/filter_bank.o ./TFLM/signal/micro/kernels/filter_bank.su ./TFLM/signal/micro/kernels/filter_bank_log.cyclo ./TFLM/signal/micro/kernels/filter_bank_log.d ./TFLM/signal/micro/kernels/filter_bank_log.o ./TFLM/signal/micro/kernels/filter_bank_log.su ./TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.cyclo ./TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.d ./TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.o ./TFLM/signal/micro/kernels/filter_bank_spectral_subtraction.su ./TFLM/signal/micro/kernels/filter_bank_square_root.cyclo ./TFLM/signal/micro/kernels/filter_bank_square_root.d ./TFLM/signal/micro/kernels/filter_bank_square_root.o ./TFLM/signal/micro/kernels/filter_bank_square_root.su ./TFLM/signal/micro/kernels/filter_bank_square_root_common.cyclo ./TFLM/signal/micro/kernels/filter_bank_square_root_common.d ./TFLM/signal/micro/kernels/filter_bank_square_root_common.o ./TFLM/signal/micro/kernels/filter_bank_square_root_common.su ./TFLM/signal/micro/kernels/framer.cyclo ./TFLM/signal/micro/kernels/framer.d ./TFLM/signal/micro/kernels/framer.o ./TFLM/signal/micro/kernels/framer.su ./TFLM/signal/micro/kernels/irfft.cyclo ./TFLM/signal/micro/kernels/irfft.d ./TFLM/signal/micro/kernels/irfft.o ./TFLM/signal/micro/kernels/irfft.su ./TFLM/signal/micro/kernels/overlap_add.cyclo ./TFLM/signal/micro/kernels/overlap_add.d ./TFLM/signal/micro/kernels/overlap_add.o ./TFLM/signal/micro/kernels/overlap_add.su ./TFLM/signal/micro/kernels/pcan.cyclo ./TFLM/signal/micro/kernels/pcan.d ./TFLM/signal/micro/kernels/pcan.o ./TFLM/signal/micro/kernels/pcan.su ./TFLM/signal/micro/kernels/rfft.cyclo ./TFLM/signal/micro/kernels/rfft.d ./TFLM/signal/micro/kernels/rfft.o ./TFLM/signal/micro/kernels/rfft.su ./TFLM/signal/micro/kernels/stacker.cyclo ./TFLM/signal/micro/kernels/stacker.d ./TFLM/signal/micro/kernels/stacker.o ./TFLM/signal/micro/kernels/stacker.su ./TFLM/signal/micro/kernels/window.cyclo ./TFLM/signal/micro/kernels/window.d ./TFLM/signal/micro/kernels/window.o ./TFLM/signal/micro/kernels/window.su

.PHONY: clean-TFLM-2f-signal-2f-micro-2f-kernels

