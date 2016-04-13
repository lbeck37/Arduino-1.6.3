//BeckLib.h, April 12, 2016
#ifndef BECKLIB_H
#define BECKLIB_H
//#include <Arduino.h>
#include <Streaming.h>
//#include <Firebase.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define LOG0    		szLogLineHeader(++lLineCount)

static const long   lSerialMonitorBaud    = 115200;
static long         lLineCount            = 0;      //Serial Monitor uses for clarity.
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

void 		SetupHttpServer(char* acHostname, ESP8266WebServer& oHttpServer, ESP8266HTTPUpdateServer& oHttpUpdateServer);
void 		HandleHttpServer(ESP8266WebServer oHttpServer);
String 		szLogLineHeader(long lLineCount);
String 		szGetTime(long lMsec);
String 		szAddZeros(int sValue, int sNumDigits);
void 		LogToSerial(String szLogString);
//void		FbaseLogLine(Firebase oFBase, String acPushPath, String szLogString);
//String 	szMakeJSONObject(String szName, String szValue);
//void		LogToBoth(Firebase oFBase, String acPushPath, String szLogString);
/*
void		LogToBoth(String szLogString, String szLogValue);
void		LogToBoth(String szLogString, int sLogValue);
void		LogToBoth(String szLogString, float fLogValue);
*/
#endif   //BECKLIB_H
