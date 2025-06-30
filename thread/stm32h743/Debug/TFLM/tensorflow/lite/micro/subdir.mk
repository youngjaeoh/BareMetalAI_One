################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/micro/fake_micro_context.cc \
../TFLM/tensorflow/lite/micro/flatbuffer_utils.cc \
../TFLM/tensorflow/lite/micro/hexdump.cc \
../TFLM/tensorflow/lite/micro/memory_helpers.cc \
../TFLM/tensorflow/lite/micro/micro_allocation_info.cc \
../TFLM/tensorflow/lite/micro/micro_allocator.cc \
../TFLM/tensorflow/lite/micro/micro_context.cc \
../TFLM/tensorflow/lite/micro/micro_interpreter.cc \
../TFLM/tensorflow/lite/micro/micro_interpreter_context.cc \
../TFLM/tensorflow/lite/micro/micro_interpreter_graph.cc \
../TFLM/tensorflow/lite/micro/micro_log.cc \
../TFLM/tensorflow/lite/micro/micro_op_resolver.cc \
../TFLM/tensorflow/lite/micro/micro_profiler.cc \
../TFLM/tensorflow/lite/micro/micro_resource_variable.cc \
../TFLM/tensorflow/lite/micro/micro_utils.cc \
../TFLM/tensorflow/lite/micro/mock_micro_graph.cc \
../TFLM/tensorflow/lite/micro/recording_micro_allocator.cc \
../TFLM/tensorflow/lite/micro/system_setup.cc \
../TFLM/tensorflow/lite/micro/test_helper_custom_ops.cc \
../TFLM/tensorflow/lite/micro/test_helpers.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/micro/fake_micro_context.d \
./TFLM/tensorflow/lite/micro/flatbuffer_utils.d \
./TFLM/tensorflow/lite/micro/hexdump.d \
./TFLM/tensorflow/lite/micro/memory_helpers.d \
./TFLM/tensorflow/lite/micro/micro_allocation_info.d \
./TFLM/tensorflow/lite/micro/micro_allocator.d \
./TFLM/tensorflow/lite/micro/micro_context.d \
./TFLM/tensorflow/lite/micro/micro_interpreter.d \
./TFLM/tensorflow/lite/micro/micro_interpreter_context.d \
./TFLM/tensorflow/lite/micro/micro_interpreter_graph.d \
./TFLM/tensorflow/lite/micro/micro_log.d \
./TFLM/tensorflow/lite/micro/micro_op_resolver.d \
./TFLM/tensorflow/lite/micro/micro_profiler.d \
./TFLM/tensorflow/lite/micro/micro_resource_variable.d \
./TFLM/tensorflow/lite/micro/micro_utils.d \
./TFLM/tensorflow/lite/micro/mock_micro_graph.d \
./TFLM/tensorflow/lite/micro/recording_micro_allocator.d \
./TFLM/tensorflow/lite/micro/system_setup.d \
./TFLM/tensorflow/lite/micro/test_helper_custom_ops.d \
./TFLM/tensorflow/lite/micro/test_helpers.d 

OBJS += \
./TFLM/tensorflow/lite/micro/fake_micro_context.o \
./TFLM/tensorflow/lite/micro/flatbuffer_utils.o \
./TFLM/tensorflow/lite/micro/hexdump.o \
./TFLM/tensorflow/lite/micro/memory_helpers.o \
./TFLM/tensorflow/lite/micro/micro_allocation_info.o \
./TFLM/tensorflow/lite/micro/micro_allocator.o \
./TFLM/tensorflow/lite/micro/micro_context.o \
./TFLM/tensorflow/lite/micro/micro_interpreter.o \
./TFLM/tensorflow/lite/micro/micro_interpreter_context.o \
./TFLM/tensorflow/lite/micro/micro_interpreter_graph.o \
./TFLM/tensorflow/lite/micro/micro_log.o \
./TFLM/tensorflow/lite/micro/micro_op_resolver.o \
./TFLM/tensorflow/lite/micro/micro_profiler.o \
./TFLM/tensorflow/lite/micro/micro_resource_variable.o \
./TFLM/tensorflow/lite/micro/micro_utils.o \
./TFLM/tensorflow/lite/micro/mock_micro_graph.o \
./TFLM/tensorflow/lite/micro/recording_micro_allocator.o \
./TFLM/tensorflow/lite/micro/system_setup.o \
./TFLM/tensorflow/lite/micro/test_helper_custom_ops.o \
./TFLM/tensorflow/lite/micro/test_helpers.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/micro/%.o TFLM/tensorflow/lite/micro/%.su TFLM/tensorflow/lite/micro/%.cyclo: ../TFLM/tensorflow/lite/micro/%.cc TFLM/tensorflow/lite/micro/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-micro

clean-TFLM-2f-tensorflow-2f-lite-2f-micro:
	-$(RM) ./TFLM/tensorflow/lite/micro/fake_micro_context.cyclo ./TFLM/tensorflow/lite/micro/fake_micro_context.d ./TFLM/tensorflow/lite/micro/fake_micro_context.o ./TFLM/tensorflow/lite/micro/fake_micro_context.su ./TFLM/tensorflow/lite/micro/flatbuffer_utils.cyclo ./TFLM/tensorflow/lite/micro/flatbuffer_utils.d ./TFLM/tensorflow/lite/micro/flatbuffer_utils.o ./TFLM/tensorflow/lite/micro/flatbuffer_utils.su ./TFLM/tensorflow/lite/micro/hexdump.cyclo ./TFLM/tensorflow/lite/micro/hexdump.d ./TFLM/tensorflow/lite/micro/hexdump.o ./TFLM/tensorflow/lite/micro/hexdump.su ./TFLM/tensorflow/lite/micro/memory_helpers.cyclo ./TFLM/tensorflow/lite/micro/memory_helpers.d ./TFLM/tensorflow/lite/micro/memory_helpers.o ./TFLM/tensorflow/lite/micro/memory_helpers.su ./TFLM/tensorflow/lite/micro/micro_allocation_info.cyclo ./TFLM/tensorflow/lite/micro/micro_allocation_info.d ./TFLM/tensorflow/lite/micro/micro_allocation_info.o ./TFLM/tensorflow/lite/micro/micro_allocation_info.su ./TFLM/tensorflow/lite/micro/micro_allocator.cyclo ./TFLM/tensorflow/lite/micro/micro_allocator.d ./TFLM/tensorflow/lite/micro/micro_allocator.o ./TFLM/tensorflow/lite/micro/micro_allocator.su ./TFLM/tensorflow/lite/micro/micro_context.cyclo ./TFLM/tensorflow/lite/micro/micro_context.d ./TFLM/tensorflow/lite/micro/micro_context.o ./TFLM/tensorflow/lite/micro/micro_context.su ./TFLM/tensorflow/lite/micro/micro_interpreter.cyclo ./TFLM/tensorflow/lite/micro/micro_interpreter.d ./TFLM/tensorflow/lite/micro/micro_interpreter.o ./TFLM/tensorflow/lite/micro/micro_interpreter.su ./TFLM/tensorflow/lite/micro/micro_interpreter_context.cyclo ./TFLM/tensorflow/lite/micro/micro_interpreter_context.d ./TFLM/tensorflow/lite/micro/micro_interpreter_context.o ./TFLM/tensorflow/lite/micro/micro_interpreter_context.su ./TFLM/tensorflow/lite/micro/micro_interpreter_graph.cyclo ./TFLM/tensorflow/lite/micro/micro_interpreter_graph.d ./TFLM/tensorflow/lite/micro/micro_interpreter_graph.o ./TFLM/tensorflow/lite/micro/micro_interpreter_graph.su ./TFLM/tensorflow/lite/micro/micro_log.cyclo ./TFLM/tensorflow/lite/micro/micro_log.d ./TFLM/tensorflow/lite/micro/micro_log.o ./TFLM/tensorflow/lite/micro/micro_log.su ./TFLM/tensorflow/lite/micro/micro_op_resolver.cyclo ./TFLM/tensorflow/lite/micro/micro_op_resolver.d ./TFLM/tensorflow/lite/micro/micro_op_resolver.o ./TFLM/tensorflow/lite/micro/micro_op_resolver.su ./TFLM/tensorflow/lite/micro/micro_profiler.cyclo ./TFLM/tensorflow/lite/micro/micro_profiler.d ./TFLM/tensorflow/lite/micro/micro_profiler.o ./TFLM/tensorflow/lite/micro/micro_profiler.su ./TFLM/tensorflow/lite/micro/micro_resource_variable.cyclo ./TFLM/tensorflow/lite/micro/micro_resource_variable.d ./TFLM/tensorflow/lite/micro/micro_resource_variable.o ./TFLM/tensorflow/lite/micro/micro_resource_variable.su ./TFLM/tensorflow/lite/micro/micro_utils.cyclo ./TFLM/tensorflow/lite/micro/micro_utils.d ./TFLM/tensorflow/lite/micro/micro_utils.o ./TFLM/tensorflow/lite/micro/micro_utils.su ./TFLM/tensorflow/lite/micro/mock_micro_graph.cyclo ./TFLM/tensorflow/lite/micro/mock_micro_graph.d ./TFLM/tensorflow/lite/micro/mock_micro_graph.o ./TFLM/tensorflow/lite/micro/mock_micro_graph.su ./TFLM/tensorflow/lite/micro/recording_micro_allocator.cyclo ./TFLM/tensorflow/lite/micro/recording_micro_allocator.d ./TFLM/tensorflow/lite/micro/recording_micro_allocator.o ./TFLM/tensorflow/lite/micro/recording_micro_allocator.su ./TFLM/tensorflow/lite/micro/system_setup.cyclo ./TFLM/tensorflow/lite/micro/system_setup.d ./TFLM/tensorflow/lite/micro/system_setup.o ./TFLM/tensorflow/lite/micro/system_setup.su ./TFLM/tensorflow/lite/micro/test_helper_custom_ops.cyclo ./TFLM/tensorflow/lite/micro/test_helper_custom_ops.d ./TFLM/tensorflow/lite/micro/test_helper_custom_ops.o ./TFLM/tensorflow/lite/micro/test_helper_custom_ops.su ./TFLM/tensorflow/lite/micro/test_helpers.cyclo ./TFLM/tensorflow/lite/micro/test_helpers.d ./TFLM/tensorflow/lite/micro/test_helpers.o ./TFLM/tensorflow/lite/micro/test_helpers.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-micro

