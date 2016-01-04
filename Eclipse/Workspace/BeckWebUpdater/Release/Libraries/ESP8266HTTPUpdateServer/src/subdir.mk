################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/Dev/_Repos/Arduino/libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp 

LINK_OBJ += \
./Libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp.o 

CPP_DEPS += \
./Libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp.o: C:/Dev/_Repos/Arduino/libraries/ESP8266HTTPUpdateServer/src/ESP8266HTTPUpdateServer.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\tools\xtensa-lx106-elf-gcc\1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0/tools/sdk/include" -c -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections -DF_CPU=80000000L -DARDUINO=10605 -DARDUINO_ESP8266_ESP12 -DARDUINO_ARCH_ESP8266  -DESP8266   -I"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0\cores\esp8266" -I"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0\variants\nodemcu" -I"C:\Dev\_Repos\Arduino\libraries\ESP8266HTTPUpdateServer" -I"C:\Dev\_Repos\Arduino\libraries\ESP8266HTTPUpdateServer\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


