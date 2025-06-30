################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.c \
../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.c \
../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.c \
../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.c \
../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.c \
../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.c 

C_DEPS += \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.d \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.d \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.d \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.d \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.d \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.d 

OBJS += \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.o \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.o \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.o \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.o \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.o \
./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.o 


# Each subdirectory must supply rules for building sources it contributes
TFLM/third_party/cmsis_nn/Source/PoolingFunctions/%.o TFLM/third_party/cmsis_nn/Source/PoolingFunctions/%.su TFLM/third_party/cmsis_nn/Source/PoolingFunctions/%.cyclo: ../TFLM/third_party/cmsis_nn/Source/PoolingFunctions/%.c TFLM/third_party/cmsis_nn/Source/PoolingFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DTFT96 -D__USE_CMSIS -DCMSIS_NN -DCMSIS_OPTIMIZATION_LEVEL -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -lnosys -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-PoolingFunctions

clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-PoolingFunctions:
	-$(RM) ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.d ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.o ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s16.su ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.d ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.o ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_get_buffer_sizes_s8.su ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.d ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.o ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s16.su ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.d ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.o ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_avgpool_s8.su ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.cyclo ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.d ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.o ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s16.su ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.cyclo ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.d ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.o ./TFLM/third_party/cmsis_nn/Source/PoolingFunctions/arm_max_pool_s8.su

.PHONY: clean-TFLM-2f-third_party-2f-cmsis_nn-2f-Source-2f-PoolingFunctions

