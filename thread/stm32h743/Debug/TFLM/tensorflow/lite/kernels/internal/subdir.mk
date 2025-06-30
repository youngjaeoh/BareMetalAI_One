################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/kernels/internal/common.cc \
../TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.cc \
../TFLM/tensorflow/lite/kernels/internal/quantization_util.cc \
../TFLM/tensorflow/lite/kernels/internal/runtime_shape.cc \
../TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.cc \
../TFLM/tensorflow/lite/kernels/internal/tensor_utils.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/kernels/internal/common.d \
./TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.d \
./TFLM/tensorflow/lite/kernels/internal/quantization_util.d \
./TFLM/tensorflow/lite/kernels/internal/runtime_shape.d \
./TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.d \
./TFLM/tensorflow/lite/kernels/internal/tensor_utils.d 

OBJS += \
./TFLM/tensorflow/lite/kernels/internal/common.o \
./TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.o \
./TFLM/tensorflow/lite/kernels/internal/quantization_util.o \
./TFLM/tensorflow/lite/kernels/internal/runtime_shape.o \
./TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.o \
./TFLM/tensorflow/lite/kernels/internal/tensor_utils.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/kernels/internal/%.o TFLM/tensorflow/lite/kernels/internal/%.su TFLM/tensorflow/lite/kernels/internal/%.cyclo: ../TFLM/tensorflow/lite/kernels/internal/%.cc TFLM/tensorflow/lite/kernels/internal/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-kernels-2f-internal

clean-TFLM-2f-tensorflow-2f-lite-2f-kernels-2f-internal:
	-$(RM) ./TFLM/tensorflow/lite/kernels/internal/common.cyclo ./TFLM/tensorflow/lite/kernels/internal/common.d ./TFLM/tensorflow/lite/kernels/internal/common.o ./TFLM/tensorflow/lite/kernels/internal/common.su ./TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.cyclo ./TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.d ./TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.o ./TFLM/tensorflow/lite/kernels/internal/portable_tensor_utils.su ./TFLM/tensorflow/lite/kernels/internal/quantization_util.cyclo ./TFLM/tensorflow/lite/kernels/internal/quantization_util.d ./TFLM/tensorflow/lite/kernels/internal/quantization_util.o ./TFLM/tensorflow/lite/kernels/internal/quantization_util.su ./TFLM/tensorflow/lite/kernels/internal/runtime_shape.cyclo ./TFLM/tensorflow/lite/kernels/internal/runtime_shape.d ./TFLM/tensorflow/lite/kernels/internal/runtime_shape.o ./TFLM/tensorflow/lite/kernels/internal/runtime_shape.su ./TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.cyclo ./TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.d ./TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.o ./TFLM/tensorflow/lite/kernels/internal/tensor_ctypes.su ./TFLM/tensorflow/lite/kernels/internal/tensor_utils.cyclo ./TFLM/tensorflow/lite/kernels/internal/tensor_utils.d ./TFLM/tensorflow/lite/kernels/internal/tensor_utils.o ./TFLM/tensorflow/lite/kernels/internal/tensor_utils.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-kernels-2f-internal

