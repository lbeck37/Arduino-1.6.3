//BeckMiniLib.h, 1/30/19
#pragma once
#include <Streaming.h>

extern const long   lMsecPerSec;
extern const long   lMsecPerMin;
extern const long   lMsecPerHour;
extern const long   lMsecPerDay;
extern const long   lSerialMonitorBaud;

#define LOG0    szLogLineHeader()

String    szGetTime           (long lMsec);
String    szAddZeros          (int sValue, int sNumDigits);
String    szLogLineHeader     (void);
//Last line.
