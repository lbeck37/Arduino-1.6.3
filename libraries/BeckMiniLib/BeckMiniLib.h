//BeckMiniLib.h, Dec 6, 2018, Lenny
#pragma once
#include <Streaming.h>
#include <Time.h>

#define LOG0    szLogLineHeader(++lLineCount)

extern long         lLineCount;
//extern const char   szVersionDate[];

//extern int          sProjectType_;

static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

String    szGetTime         (long lMsec);
String    szAddZeros        (int sValue, int sNumDigits);
String    szLogLineHeader   (long lLineCount);

/*Stripped to use in fauxmo.cpp the Arduino library
extern const int       sUpButtonPin;
extern const int       sDownButtonPin;
extern const byte      cI2C_SDA_Pin;
extern const byte      cI2C_SCL_Pin;
extern const byte      cSPI_A0CmdData_Pin;
extern const byte      cSPI_MISO_Pin;
extern const byte      cSPI_MOSI_Pin;
extern const byte      cSPI_CLK_Pin;
extern const byte      cSPI_Select_Pin;
extern const int       sServoPin;
extern const byte      cBogusResetPin;
extern const byte      cHW_SPI;
*/
