//BeckMiniLib.h, 1/20/19
#pragma once
#include <Streaming.h>
#include <Time.h>

extern const long		lMsecPerSec;
extern const long		lMsecPerMin;
extern const long		lMsecPerHour;
extern const long		lMsecPerDay;
extern const long   lSerialMonitorBaud;

//extern long         lLineCount;

#define LOG0    szLogLineHeader()

/*
static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;
*/

String    szGetTime           (long lMsec);
String    szAddZeros          (int sValue, int sNumDigits);
String    szLogLineHeader     (void);

String    szFormatTimeString  (void);
String    szFormatDateString  (void);
//Last line.
