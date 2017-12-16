//BeckMiniLib.h, 12/16/17
#ifndef BECKMINILIB_H
#define BECKMINILIB_H
#include <Streaming.h>
#include <Time.h>
//#include <Adafruit_ADS1015.h>   //Just for the defines

#define INT8        int8_t
#define UINT8       uint8_t
#define INT16       int16_t
#define UINT16      uint16_t
#define INT32       int32_t
#define UINT32      uint32_t
#define INT64       int64_t
#define UINT64      uint64_t

#define LOG0        szLogLineHeader(++lLineCount)
#define BLog        LogJustToSerial
#define BLogS       LogJustToSerial

extern const char 	szVersionDate[];

extern int					sProjectType_;

static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

extern bool           bDebugLog;    //Used to limit number of printouts.

//Function protos
void      LogJustToSerial(String sLogline);
String    szLogLineHeader(long lLineCount);
String    szGetTime(long lMsec);
String    szAddZeros(int sValue, int sNumDigits);


//Global variables
extern long                     lLineCount;       //Serial Monitor uses for clarity.
extern String                   szLogLine;

extern void LogToBoth(String szLogString);
extern void LogToBoth(String szLogString, String szLogValue);
extern void LogToBoth(String szLogString, int sLogValue);
extern void LogToBoth(String szLogString, UINT32 uwLogValue);
extern void LogToBoth(String szLogString, float fLogValue);
extern void LogToBoth(String szLogString, double dLogValue);

extern void LogToSerial(String szLogString);
extern void LogToSerial(String szLogString, String szLogValue);
extern void LogToSerial(String szLogString, int sLogValue);
extern void LogToSerial(String szLogString, UINT32 uwLogValue);
extern void LogToSerial(String szLogString, float fLogValue);
extern void LogToSerial(String szLogString, double dLogValue);

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

#endif   //BECKMINILIB_H
