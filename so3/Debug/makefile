################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include scripts/kconfig/lxdialog/subdir.mk
-include scripts/kconfig/subdir.mk
-include scripts/basic/subdir.mk
-include mm/subdir.mk
-include lib/libfdt/subdir.mk
-include lib/subdir.mk
-include kernel/subdir.mk
-include ipc/subdir.mk
-include fs/fat/subdir.mk
-include fs/subdir.mk
-include devices/timer/subdir.mk
-include devices/serial/subdir.mk
-include devices/ramdev/subdir.mk
-include devices/mmc/subdir.mk
-include devices/irq/subdir.mk
-include devices/i2c/subdir.mk
-include devices/subdir.mk
-include arch/arm/boot/subdir.mk
-include arch/arm/subdir.mk
-include apps/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: so3

# Tool invocations
so3: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "so3" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) so3
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
