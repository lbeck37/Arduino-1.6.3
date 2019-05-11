//BeckLogLib.h, 1/26/19
#pragma once
#include <BeckMiniLib.h>
#include <Streaming.h>
#include <Time.h>

#define LOG0        szLogLineHeader(++lLineCount)
#define BLog        LogToSerial

#define INT8        int8_t
#define UINT8       uint8_t
#define INT16       int16_t
#define UINT16      uint16_t
#define INT32       int32_t
#define UINT32      uint32_t
#define INT64       int64_t
#define UINT64      uint64_t
#define LOG0        szLogLineHeader(++lLineCount)

//Global variables
extern long         lLineCount;       //Serial Monitor uses for clarity.
extern String       szLogLine;
extern bool         bDebugLog;        //Used to limit number of printouts.

//Function protos
void      LogJustToSerial(String sLogline);
String    szLogLineHeader(long lLineCount);
String    szAddZeros(int sValue, int sNumDigits);
void LogToBoth    (String szLogString);
void LogToBoth    (String szLogString, String szLogValue);
void LogToBoth    (String szLogString, int sLogValue);
void LogToSerial  (String szLogString, int sValue1, int sValue2, int sValue3);
void LogToSerial  (String szLogString, UINT16 usLogValue);
void LogToBoth    (String szLogString, UINT32 uwLogValue);
void LogToBoth    (String szLogString, float fLogValue);
void LogToBoth    (String szLogString, double dLogValue);

void LogToSerial  (String szLogString);
void LogToSerial  (String szLogString, String szLogValue);
void LogToSerial  (String szLogString, int sLogValue);
void LogToSerial  (String szLogString, int sValue1, int sValue2, int sValue3);
void LogToSerial  (String szLogString, UINT16 usLogValue);
void LogToSerial  (String szLogString, UINT32 uwLogValue);
void LogToSerial  (String szLogString, float fLogValue);
void LogToSerial  (String szLogString, double dLogValue);
//Last line.
