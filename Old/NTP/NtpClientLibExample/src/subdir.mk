################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Dev\_Repos\Arduino\libraries\NtpClientLib\src\AvrNTPClient.cpp \
C:\Dev\_Repos\Arduino\libraries\NtpClientLib\src\ESPNTPClient.cpp 

LINK_OBJ += \
.\libraries\NtpClientLib\src\AvrNTPClient.cpp.o \
.\libraries\NtpClientLib\src\ESPNTPClient.cpp.o 

CPP_DEPS += \
.\libraries\NtpClientLib\src\AvrNTPClient.cpp.d \
.\libraries\NtpClientLib\src\ESPNTPClient.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\NtpClientLib\src\AvrNTPClient.cpp.o: C:\Dev\_Repos\Arduino\libraries\NtpClientLib\src\AvrNTPClient.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\_eclipseSloeber_4.2\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\_eclipseSloeber_4.2\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.3.0/tools/sdk/include" "-IC:\_eclipseSloeber_4.2\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.3.0/tools/sdk/lwip/include" "-IC:\_eclipseSloeber_4.2\workspace\BeckE8266_NtpTimeExample/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266" -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\variants\nodemcu" -I"C:\Dev\_Repos\Arduino\libraries\BeckMiniLib" -I"C:\Dev\_Repos\Arduino\libraries\BeckNtpLib" -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi" -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi\src" -I"C:\Dev\_Repos\Arduino\libraries\Time" -I"C:\Dev\_Repos\Arduino\libraries\Streaming" -I"C:\Dev\_Repos\Arduino\libraries\NtpClient" -I"C:\Dev\_Repos\Arduino\libraries\NtpClient\src" -I"C:\Dev\_Repos\Arduino\libraries\NtpClientLib" -I"C:\Dev\_Repos\Arduino\libraries\NtpClientLib\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\NtpClientLib\src\ESPNTPClient.cpp.o: C:\Dev\_Repos\Arduino\libraries\NtpClientLib\src\ESPNTPClient.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\_eclipseSloeber_4.2\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-IC:\_eclipseSloeber_4.2\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.3.0/tools/sdk/include" "-IC:\_eclipseSloeber_4.2\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.3.0/tools/sdk/lwip/include" "-IC:\_eclipseSloeber_4.2\workspace\BeckE8266_NtpTimeExample/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\cores\esp8266" -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\variants\nodemcu" -I"C:\Dev\_Repos\Arduino\libraries\BeckMiniLib" -I"C:\Dev\_Repos\Arduino\libraries\BeckNtpLib" -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi" -I"C:\_eclipseSloeber_4.2\arduinoPlugin\packages\esp8266\hardware\esp8266\2.3.0\libraries\ESP8266WiFi\src" -I"C:\Dev\_Repos\Arduino\libraries\Time" -I"C:\Dev\_Repos\Arduino\libraries\Streaming" -I"C:\Dev\_Repos\Arduino\libraries\NtpClient" -I"C:\Dev\_Repos\Arduino\libraries\NtpClient\src" -I"C:\Dev\_Repos\Arduino\libraries\NtpClientLib" -I"C:\Dev\_Repos\Arduino\libraries\NtpClientLib\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


