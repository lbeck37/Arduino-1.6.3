//BeckMiniLib.h, 1/27/19
#pragma once
#include <Streaming.h>
//#include <Time.h>

extern const long		lMsecPerSec;
extern const long		lMsecPerMin;
extern const long		lMsecPerHour;
extern const long		lMsecPerDay;
extern const long   lSerialMonitorBaud;

//extern long         lLineCount;

#define LOG0    szLogLineHeader()

String    szGetTime           (long lMsec);
String    szAddZeros          (int sValue, int sNumDigits);
String    szLogLineHeader     (void);

String    szFormatTimeString  (void);
String    szFormatDateString  (void);
//Last line.
