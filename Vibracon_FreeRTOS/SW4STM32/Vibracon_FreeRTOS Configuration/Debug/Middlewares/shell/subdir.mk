################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/shell/shell.c 

OBJS += \
./Middlewares/shell/shell.o 

C_DEPS += \
./Middlewares/shell/shell.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/shell/%.o: ../Middlewares/shell/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F107xC -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/Httpd" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/FTP Server" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/shell" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Inc" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/CoffeFlashSystem" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/FreeModbus/tcp" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/FreeModbus/port" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/FreeModbus/include" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/SW4STM32/Vibracon_FreeRTOS Configuration/Middlewares/FreeModbus" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/system" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include/ipv4" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Drivers/STM32F1xx_HAL_Driver/Inc" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/FreeRTOS/Source/include" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/system/arch" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include/ipv4/lwip" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include/lwip" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include/netif" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include/posix" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Middlewares/Third_Party/LwIP/src/netif/ppp" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Drivers/CMSIS/Include" -I"G:/Work/Cubetech/SitaniecTech/Firmware/Vibracon_FreeRTOS/Drivers/CMSIS/Device/ST/STM32F1xx/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


