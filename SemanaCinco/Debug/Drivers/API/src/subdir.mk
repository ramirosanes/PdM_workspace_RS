################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/src/API_uart.c \
../Drivers/API/src/API_utils.c 

OBJS += \
./Drivers/API/src/API_uart.o \
./Drivers/API/src/API_utils.o 

C_DEPS += \
./Drivers/API/src/API_uart.d \
./Drivers/API/src/API_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/src/%.o Drivers/API/src/%.su Drivers/API/src/%.cyclo: ../Drivers/API/src/%.c Drivers/API/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/rami/STM32Cube/PdM_workspace_RS/SemanaCinco/Drivers/API" -I"/home/rami/STM32Cube/PdM_workspace_RS/SemanaCinco/Drivers/API/inc" -I"/home/rami/STM32Cube/PdM_workspace_RS/SemanaCinco/Drivers/API/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-src

clean-Drivers-2f-API-2f-src:
	-$(RM) ./Drivers/API/src/API_uart.cyclo ./Drivers/API/src/API_uart.d ./Drivers/API/src/API_uart.o ./Drivers/API/src/API_uart.su ./Drivers/API/src/API_utils.cyclo ./Drivers/API/src/API_utils.d ./Drivers/API/src/API_utils.o ./Drivers/API/src/API_utils.su

.PHONY: clean-Drivers-2f-API-2f-src

