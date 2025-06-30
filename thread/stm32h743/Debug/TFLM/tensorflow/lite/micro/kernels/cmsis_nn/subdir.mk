################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.cc \
../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.cc 

CC_DEPS += \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.d \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.d 

OBJS += \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.o \
./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/tensorflow/lite/micro/kernels/cmsis_nn/%.o TFLM/tensorflow/lite/micro/kernels/cmsis_nn/%.su TFLM/tensorflow/lite/micro/kernels/cmsis_nn/%.cyclo: ../TFLM/tensorflow/lite/micro/kernels/cmsis_nn/%.cc TFLM/tensorflow/lite/micro/kernels/cmsis_nn/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -D__USE_CMSIS -DSTM32H743xx -DCMSIS_OPTIMIZATION_LEVEL -DCMSIS_NN -DTFT96 -DUSE_HAL_DRIVER -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-kernels-2f-cmsis_nn

clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-kernels-2f-cmsis_nn:
	-$(RM) ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/add.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/conv.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/mul.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pad.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/pooling.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/softmax.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/svdf.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.su ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.cyclo ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.d ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.o ./TFLM/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.su

.PHONY: clean-TFLM-2f-tensorflow-2f-lite-2f-micro-2f-kernels-2f-cmsis_nn

