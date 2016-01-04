################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
arduino.ar \

ELF += \
BeckHello.elf \

EEP += \
BeckHello.eep \

FLASH_IMAGE += \
BeckHello.bin \


# All Target
all: BeckHello

# Tool invocations
BeckHello: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-size" -A "C:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release/BeckHello.elf"
	@echo 'Finished building target: $@'
	@echo ' '

arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-ar" cru  "C:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

BeckHello.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -g -Os -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static "-LC:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0/tools/sdk/lib" "-LC:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0/tools/sdk/ld" "-Teagle.flash.4m1m.ld" -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy  -o "C:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release/BeckHello.elf" -Wl,--start-group   $(LINK_OBJ) $(AR)  "C:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release/arduino.ar" -lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig -lwps -lcrypto -laxtls -Wl,--end-group  "-LC:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release"
	@echo 'Finished building: $@'
	@echo ' '

BeckHello.eep: $(ELF)
	@echo 'Create eeprom image'
	${A.RECIPE.OBJCOPY.EEP.PATTERN}
	@echo 'Finished building: $@'
	@echo ' '

BeckHello.bin: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\tools\esptool\0.4.6/esptool.exe" -eo "C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0/bootloaders/eboot/eboot.elf" -bo "C:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release/BeckHello.bin" -bm dio -bf 40 -bz 4M -bs .text -bp 4096 -ec -eo "C:/Dev/_Repos/Arduino/Eclipse/Workspace/BeckHello/Release/BeckHello.elf" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(C_DEPS)$(AR)$(EEP)$(FLASH_IMAGE)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS) BeckHello
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
