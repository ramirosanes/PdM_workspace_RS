################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/APP/src/APP_clock.c \
../Drivers/APP/src/APP_display.c 

OBJS += \
./Drivers/APP/src/APP_clock.o \
./Drivers/APP/src/APP_display.o 

C_DEPS += \
./Drivers/APP/src/APP_clock.d \
./Drivers/APP/src/APP_display.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/APP/src/%.o Drivers/APP/src/%.su Drivers/APP/src/%.cyclo: ../Drivers/APP/src/%.c Drivers/APP/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/API" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/API/inc" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/API/src" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/APP" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/APP/inc" -I"/home/rami/STM32Cube/PdM_workspace_RS/Final/Drivers/APP/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-APP-2f-src

clean-Drivers-2f-APP-2f-src:
	-$(RM) ./Drivers/APP/src/APP_clock.cyclo ./Drivers/APP/src/APP_clock.d ./Drivers/APP/src/APP_clock.o ./Drivers/APP/src/APP_clock.su ./Drivers/APP/src/APP_display.cyclo ./Drivers/APP/src/APP_display.d ./Drivers/APP/src/APP_display.o ./Drivers/APP/src/APP_display.su

.PHONY: clean-Drivers-2f-APP-2f-src

