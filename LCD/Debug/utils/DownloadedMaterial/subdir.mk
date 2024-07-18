################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utils/DownloadedMaterial/DWT_Delay.c 

OBJS += \
./utils/DownloadedMaterial/DWT_Delay.o 

C_DEPS += \
./utils/DownloadedMaterial/DWT_Delay.d 


# Each subdirectory must supply rules for building sources it contributes
utils/DownloadedMaterial/%.o utils/DownloadedMaterial/%.su utils/DownloadedMaterial/%.cyclo: ../utils/DownloadedMaterial/%.c utils/DownloadedMaterial/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/rami/STM32Cube/PdM_workspace_RS/LCD/Core/Src" -I"/home/rami/STM32Cube/PdM_workspace_RS/LCD/API" -I"/home/rami/STM32Cube/PdM_workspace_RS/LCD/API/inc" -I"/home/rami/STM32Cube/PdM_workspace_RS/LCD/API/src" -I"/home/rami/STM32Cube/PdM_workspace_RS/LCD/utils/inc" -I"/home/rami/STM32Cube/PdM_workspace_RS/LCD/utils/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-utils-2f-DownloadedMaterial

clean-utils-2f-DownloadedMaterial:
	-$(RM) ./utils/DownloadedMaterial/DWT_Delay.cyclo ./utils/DownloadedMaterial/DWT_Delay.d ./utils/DownloadedMaterial/DWT_Delay.o ./utils/DownloadedMaterial/DWT_Delay.su

.PHONY: clean-utils-2f-DownloadedMaterial

