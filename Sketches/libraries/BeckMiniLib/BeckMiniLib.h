//BeckMiniLib.h, 2/3/19
#pragma once
#include <Streaming.h>

#define LOG0    szLogLineHeader()

extern const long   lMsecPerSec;
extern const long   lMsecPerMin;
extern const long   lMsecPerHour;
extern const long   lMsecPerDay;
extern const long   lSerialMonitorBaud;

String    szAddZeros            (int sValue, int sNumDigits);
String    szLogLineHeader       (void);
time_t    lGetLocalTime         (void);
String    szFormatTimeString    (time_t lBoiseTime);
String    szFormatDateString    (time_t lLocalTime);
//Last line.
