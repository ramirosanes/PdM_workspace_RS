################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_debounce.c \
../Drivers/API/src/API_delay.c \
../Drivers/API/src/API_init.c \
../Drivers/API/src/API_lcd.c \
../Drivers/API/src/API_leds.c \
../Drivers/API/src/gpio.c \
../Drivers/API/src/i2c.c \
../Drivers/API/src/stm32f4xx_hal_msp.c 

OBJS += \
./Drivers/API/src/API_debounce.o \
./Drivers/API/src/API_delay.o \
./Drivers/API/src/API_init.o \
./Drivers/API/src/API_lcd.o \
./Drivers/API/src/API_leds.o \
./Drivers/API/src/gpio.o \
./Drivers/API/src/i2c.o \
./Drivers/API/src/stm32f4xx_hal_msp.o 

C_DEPS += \
./Drivers/API/src/API_debounce.d \
./Drivers/API/src/API_delay.d \
./Drivers/API/src/API_init.d \
./Drivers/API/src/API_lcd.d \
./Drivers/API/src/API_leds.d \
./Drivers/API/src/gpio.d \
./Drivers/API/src/i2c.d \
./Drivers/API/src/stm32f4xx_hal_msp.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su Drivers/API/src/%.cyclo: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/API" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/API/inc" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/API/src" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Core/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_debounce.cyclo ./Drivers/API/src/API_debounce.d ./Drivers/API/src/API_debounce.o ./Drivers/API/src/API_debounce.su ./Drivers/API/src/API_delay.cyclo ./Drivers/API/src/API_delay.d ./Drivers/API/src/API_delay.o ./Drivers/API/src/API_delay.su ./Drivers/API/src/API_init.cyclo ./Drivers/API/src/API_init.d ./Drivers/API/src/API_init.o ./Drivers/API/src/API_init.su ./Drivers/API/src/API_lcd.cyclo ./Drivers/API/src/API_lcd.d ./Drivers/API/src/API_lcd.o ./Drivers/API/src/API_lcd.su ./Drivers/API/src/API_leds.cyclo ./Drivers/API/src/API_leds.d ./Drivers/API/src/API_leds.o ./Drivers/API/src/API_leds.su ./Drivers/API/src/gpio.cyclo ./Drivers/API/src/gpio.d ./Drivers/API/src/gpio.o ./Drivers/API/src/gpio.su ./Drivers/API/src/i2c.cyclo ./Drivers/API/src/i2c.d ./Drivers/API/src/i2c.o ./Drivers/API/src/i2c.su ./Drivers/API/src/stm32f4xx_hal_msp.cyclo ./Drivers/API/src/stm32f4xx_hal_msp.d ./Drivers/API/src/stm32f4xx_hal_msp.o ./Drivers/API/src/stm32f4xx_hal_msp.su

.PHONY: clean-Drivers-2f-API-2f-src

