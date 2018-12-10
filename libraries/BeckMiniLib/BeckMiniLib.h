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
//Last line.
