################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc \
../TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.cc \
../TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.d \
./TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.d \
./TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.d 

OBJS += \
./TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.o \
./TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.o \
./TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/micro/memory_planner/%.o TFLM/tensorflow/lite/micro/memory_planner/%.su TFLM/tensorflow/lite/micro/memory_planner/%.cyclo: ../TFLM/tensorflow/lite/micro/memory_planner/%.cc TFLM/tensorflow/lite/micro/memory_planner/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-memory_planner

clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-memory_planner:
	-$(RM) ./TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cyclo ./TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.d ./TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.o ./TFLM/tensorflow/lite/micro/memory_planner/greedy_memory_planner.su ./TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.cyclo ./TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.d ./TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.o ./TFLM/tensorflow/lite/micro/memory_planner/linear_memory_planner.su ./TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cyclo ./TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.d ./TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.o ./TFLM/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-memory_planner

