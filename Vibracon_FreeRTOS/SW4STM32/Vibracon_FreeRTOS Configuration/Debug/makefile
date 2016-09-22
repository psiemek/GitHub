################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/shell/subdir.mk
-include Middlewares/LwIP/subdir.mk
-include Middlewares/Httpd/subdir.mk
-include Middlewares/FreeRTOS/subdir.mk
-include Middlewares/FreeModbus/tcp/subdir.mk
-include Middlewares/FreeModbus/port/subdir.mk
-include Middlewares/FreeModbus/functions/subdir.mk
-include Middlewares/FreeModbus/subdir.mk
-include Middlewares/FTP\ Server/subdir.mk
-include Middlewares/CoffeFlashSystem/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Application/User/subdir.mk
-include Application/SW4STM32/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Vibracon_FreeRTOS\ Configuration.elf

# Tool invocations
Vibracon_FreeRTOS\ Configuration.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -specs=nosys.specs -specs=nano.specs -T"../STM32F107VCTx_FLASH.ld" -Wl,-Map=output.map -Wl,--gc-sections -lm -o "Vibracon_FreeRTOS Configuration.elf" @"objects.list" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "Vibracon_FreeRTOS Configuration.elf" "Vibracon_FreeRTOS Configuration.bin"
	arm-none-eabi-size "Vibracon_FreeRTOS Configuration.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
