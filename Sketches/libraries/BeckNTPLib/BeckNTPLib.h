//BeckNTPLib.h, Beck 1/27/19
#pragma once
/*
#include <NtpClientLib.h>
//#include <NtpClient.h>
*/
//#include <TimeLib.h>
#include <Timezone.h>

//US Mountain Time Zone (Boise)
extern TimeChangeRule   oMDT_Rule;	//Mountain Daylight Time = UTC - 7 hours
extern TimeChangeRule   oMST_Rule;  //Mountain Standard Time = UTC - 8 hours
extern Timezone         oMT_Timezone;
extern TimeChangeRule*  pTimeChangeRule;

void    SetupNTP            (void);
/*
void    onSTAGotIP          (WiFiEventStationModeGotIP ipInfo);
void    onSTADisconnected   (WiFiEventStationModeDisconnected event_info);
String  szFormatTimeString  (void);
String  szFormatDateString  (void);
String  szPrintDigits       (int digits);
*/
/*
String  szFormatTimeString  (void);
String  szFormatDateString  (void);
*/
//Last line.
