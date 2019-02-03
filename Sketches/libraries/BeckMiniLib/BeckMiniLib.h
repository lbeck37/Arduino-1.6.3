//BeckMiniLib.h, 2/3/19
#pragma once
#include <Streaming.h>

#define LOG0    szLogLineHeader()

extern const long   lMsecPerSec;
extern const long   lMsecPerMin;
extern const long   lMsecPerHour;
extern const long   lMsecPerDay;
extern const long   lSerialMonitorBaud;

String    szAddZeros          (int sValue, int sNumDigits);
String    szLogLineHeader     (void);
//Last line.
