################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/ST7735/lcd.c \
../Drivers/BSP/ST7735/logo.c \
../Drivers/BSP/ST7735/logo_128_160.c \
../Drivers/BSP/ST7735/logo_160_80.c \
../Drivers/BSP/ST7735/st7735.c \
../Drivers/BSP/ST7735/st7735_reg.c 

C_DEPS += \
./Drivers/BSP/ST7735/lcd.d \
./Drivers/BSP/ST7735/logo.d \
./Drivers/BSP/ST7735/logo_128_160.d \
./Drivers/BSP/ST7735/logo_160_80.d \
./Drivers/BSP/ST7735/st7735.d \
./Drivers/BSP/ST7735/st7735_reg.d 

OBJS += \
./Drivers/BSP/ST7735/lcd.o \
./Drivers/BSP/ST7735/logo.o \
./Drivers/BSP/ST7735/logo_128_160.o \
./Drivers/BSP/ST7735/logo_160_80.o \
./Drivers/BSP/ST7735/st7735.o \
./Drivers/BSP/ST7735/st7735_reg.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/ST7735/%.o Drivers/BSP/ST7735/%.su Drivers/BSP/ST7735/%.cyclo: ../Drivers/BSP/ST7735/%.c Drivers/BSP/ST7735/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DTFT96 -D__USE_CMSIS -DCMSIS_NN -DCMSIS_OPTIMIZATION_LEVEL -DSTM32 -DSTM32H7SINGLE -DSTM32H7 -DSTM32H743VITx -c -I../Inc -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Device/ST/STM32H7xx/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/STM32H7xx_HAL_Driver/Inc/Legacy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/CMSIS/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/Camera" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/Drivers/BSP/ST7735" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/kissfft" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/gemmlowp" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/ruy" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/flatbuffers/include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn/Include" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM/third_party/cmsis_nn" -I"C:/Users/jashe/Documents/github/BareMetalAI_One/thread/stm32h743/TFLM" -O3 -ffunction-sections -fdata-sections -lnosys -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-ST7735

clean-Drivers-2f-BSP-2f-ST7735:
	-$(RM) ./Drivers/BSP/ST7735/lcd.cyclo ./Drivers/BSP/ST7735/lcd.d ./Drivers/BSP/ST7735/lcd.o ./Drivers/BSP/ST7735/lcd.su ./Drivers/BSP/ST7735/logo.cyclo ./Drivers/BSP/ST7735/logo.d ./Drivers/BSP/ST7735/logo.o ./Drivers/BSP/ST7735/logo.su ./Drivers/BSP/ST7735/logo_128_160.cyclo ./Drivers/BSP/ST7735/logo_128_160.d ./Drivers/BSP/ST7735/logo_128_160.o ./Drivers/BSP/ST7735/logo_128_160.su ./Drivers/BSP/ST7735/logo_160_80.cyclo ./Drivers/BSP/ST7735/logo_160_80.d ./Drivers/BSP/ST7735/logo_160_80.o ./Drivers/BSP/ST7735/logo_160_80.su ./Drivers/BSP/ST7735/st7735.cyclo ./Drivers/BSP/ST7735/st7735.d ./Drivers/BSP/ST7735/st7735.o ./Drivers/BSP/ST7735/st7735.su ./Drivers/BSP/ST7735/st7735_reg.cyclo ./Drivers/BSP/ST7735/st7735_reg.d ./Drivers/BSP/ST7735/st7735_reg.o ./Drivers/BSP/ST7735/st7735_reg.su

.PHONY: clean-Drivers-2f-BSP-2f-ST7735

