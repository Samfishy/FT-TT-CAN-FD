################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := TTCANFD1_WithSec_MasterWithProto
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
TTCANFD1_WithSec_MasterWithProto.elf \

MAP_FILES += \
TTCANFD1_WithSec_MasterWithProto.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
TTCANFD1_WithSec_MasterWithProto.list \


# All Target
all: main-build

# Main-build Target
main-build: TTCANFD1_WithSec_MasterWithProto.elf secondary-outputs

# Tool invocations
TTCANFD1_WithSec_MasterWithProto.elf TTCANFD1_WithSec_MasterWithProto.map: $(OBJS) $(USER_OBJS) /home/samfishy/Documents/STM_Workspace_Thapar/TTCANFD1_WithSec_MasterWithProto/STM32G491RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "TTCANFD1_WithSec_MasterWithProto.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/samfishy/Documents/STM_Workspace_Thapar/TTCANFD1_WithSec_MasterWithProto/STM32G491RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="TTCANFD1_WithSec_MasterWithProto.map" -Wl,--gc-sections -static -L/home/samfishy/Downloads/x-cube-cryptolib-v4-5-0/STM32CubeExpansion_Crypto_V4.5.0/Middlewares/ST/STM32_Cryptographic/lib --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

TTCANFD1_WithSec_MasterWithProto.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "TTCANFD1_WithSec_MasterWithProto.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) TTCANFD1_WithSec_MasterWithProto.elf TTCANFD1_WithSec_MasterWithProto.list TTCANFD1_WithSec_MasterWithProto.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
