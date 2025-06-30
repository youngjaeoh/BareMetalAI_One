################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.cc \
../TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.d \
./TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.d 

OBJS += \
./TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.o \
./TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/micro/cortex_m_generic/%.o TFLM/tensorflow/lite/micro/cortex_m_generic/%.su TFLM/tensorflow/lite/micro/cortex_m_generic/%.cyclo: ../TFLM/tensorflow/lite/micro/cortex_m_generic/%.cc TFLM/tensorflow/lite/micro/cortex_m_generic/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-cortex_m_generic

clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-cortex_m_generic:
	-$(RM) ./TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.cyclo ./TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.d ./TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.o ./TFLM/tensorflow/lite/micro/cortex_m_generic/debug_log.su ./TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.cyclo ./TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.d ./TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.o ./TFLM/tensorflow/lite/micro/cortex_m_generic/micro_time.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-cortex_m_generic

