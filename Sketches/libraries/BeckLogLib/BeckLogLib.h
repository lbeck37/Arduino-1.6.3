//BeckLogLib.h, 1/26/19
#pragma once
#include <BeckMiniLib.h>
#include <Streaming.h>
#include <Time.h>

//#define LOG0        szLogLineHeader(++lLineCount)
#define LOG0        szLogLineHeader()

void      LogToSerial           (String szLogString);
void      LogToSerial           (String szLogString, String szLogValue);
void      LogToSerial           (String szLogString, int sLogValue);
void      LogToSerial           (String szLogString, float fLogValue);
String    szAddZeros            (int sValue, int sNumDigits);
String    szLogLineHeader       (void);
time_t    lGetLocalTime         (void);
String    szFormatTimeString    (time_t lBoiseTime);
String    szFormatDateString    (time_t lLocalTime);

//Last line.
