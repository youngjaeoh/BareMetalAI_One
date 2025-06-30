################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Camera/camera.c \
../Drivers/BSP/Camera/ov2640.c \
../Drivers/BSP/Camera/ov2640_regs.c \
../Drivers/BSP/Camera/ov5640.c \
../Drivers/BSP/Camera/ov5640_regs.c \
../Drivers/BSP/Camera/ov7670.c \
../Drivers/BSP/Camera/ov7670_regs.c \
../Drivers/BSP/Camera/ov7725.c \
../Drivers/BSP/Camera/ov7725_regs.c 

C_DEPS += \
./Drivers/BSP/Camera/camera.d \
./Drivers/BSP/Camera/ov2640.d \
./Drivers/BSP/Camera/ov2640_regs.d \
./Drivers/BSP/Camera/ov5640.d \
./Drivers/BSP/Camera/ov5640_regs.d \
./Drivers/BSP/Camera/ov7670.d \
./Drivers/BSP/Camera/ov7670_regs.d \
./Drivers/BSP/Camera/ov7725.d \
./Drivers/BSP/Camera/ov7725_regs.d 

OBJS += \
./Drivers/BSP/Camera/camera.o \
./Drivers/BSP/Camera/ov2640.o \
./Drivers/BSP/Camera/ov2640_regs.o \
./Drivers/BSP/Camera/ov5640.o \
./Drivers/BSP/Camera/ov5640_regs.o \
./Drivers/BSP/Camera/ov7670.o \
./Drivers/BSP/Camera/ov7670_regs.o \
./Drivers/BSP/Camera/ov7725.o \
./Drivers/BSP/Camera/ov7725_regs.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Camera/%.o Drivers/BSP/Camera/%.su Drivers/BSP/Camera/%.cyclo: ../Drivers/BSP/Camera/%.c Drivers/BSP/Camera/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DTFT96 -D__USE_CMSIS -DCMSIS_NN -DCMSIS_OPTIMIZATION_LEVEL -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -lnosys -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Camera

clean-Drivers-2f-BSP-2f-Camera:
	-$(RM) ./Drivers/BSP/Camera/camera.cyclo ./Drivers/BSP/Camera/camera.d ./Drivers/BSP/Camera/camera.o ./Drivers/BSP/Camera/camera.su ./Drivers/BSP/Camera/ov2640.cyclo ./Drivers/BSP/Camera/ov2640.d ./Drivers/BSP/Camera/ov2640.o ./Drivers/BSP/Camera/ov2640.su ./Drivers/BSP/Camera/ov2640_regs.cyclo ./Drivers/BSP/Camera/ov2640_regs.d ./Drivers/BSP/Camera/ov2640_regs.o ./Drivers/BSP/Camera/ov2640_regs.su ./Drivers/BSP/Camera/ov5640.cyclo ./Drivers/BSP/Camera/ov5640.d ./Drivers/BSP/Camera/ov5640.o ./Drivers/BSP/Camera/ov5640.su ./Drivers/BSP/Camera/ov5640_regs.cyclo ./Drivers/BSP/Camera/ov5640_regs.d ./Drivers/BSP/Camera/ov5640_regs.o ./Drivers/BSP/Camera/ov5640_regs.su ./Drivers/BSP/Camera/ov7670.cyclo ./Drivers/BSP/Camera/ov7670.d ./Drivers/BSP/Camera/ov7670.o ./Drivers/BSP/Camera/ov7670.su ./Drivers/BSP/Camera/ov7670_regs.cyclo ./Drivers/BSP/Camera/ov7670_regs.d ./Drivers/BSP/Camera/ov7670_regs.o ./Drivers/BSP/Camera/ov7670_regs.su ./Drivers/BSP/Camera/ov7725.cyclo ./Drivers/BSP/Camera/ov7725.d ./Drivers/BSP/Camera/ov7725.o ./Drivers/BSP/Camera/ov7725.su ./Drivers/BSP/Camera/ov7725_regs.cyclo ./Drivers/BSP/Camera/ov7725_regs.d ./Drivers/BSP/Camera/ov7725_regs.o ./Drivers/BSP/Camera/ov7725_regs.su

.PHONY: clean-Drivers-2f-BSP-2f-Camera

