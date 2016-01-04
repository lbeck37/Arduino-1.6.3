################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../.ino.cpp 

LINK_OBJ += \
./.ino.cpp.o 

CPP_DEPS += \
./.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ../.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0\cores\esp8266" -I"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0\variants\nodemcu" -I"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0\libraries\ESP8266HTTPUpdateServer" -I"C:\Users\Larry\AppData\Roaming\Arduino15\packages\esp8266\hardware\esp8266\2.0.0\libraries\ESP8266HTTPUpdateServer\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


