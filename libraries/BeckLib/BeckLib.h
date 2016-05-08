//BeckLib.h, May 7, 2016
#ifndef BECKLIB_H
#define BECKLIB_H
//#include <Arduino.h>
#include <Streaming.h>
#include <Firebase.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define LOG0    		szLogLineHeader(++lLineCount)

static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

class BeckFirebase {
public:
	BeckFirebase(String sDatabaseURL,String sFirebaseSecret, String sPushPath, String sMyName);
/*
	BeckFirebase() {};
	~BeckFirebase() {};
*/
	void 		LogToSerial(String sLogline);
	void		LogToFirebase(String sLogline);
	void		LogToBoth(String sLogline);
	String		GetDatabaseURL(void);
	String		GetLogPath(void);
	String		GetPushPath(void);
protected:
	String		sDatabaseURL_;
	String		strFirebaseSecret_;
	//Firebase 	oFBase_;
	Firebase* 	pFBase_;
	String		sLogPath_;
	String 		sMyName_;
	String		sPushPath_;

	String		sMakeJSONObject(String sName, String sValue);
};	//BeckFirebase


BeckFirebase*	StartBeckFirebase(String sDatabaseURL, String sFirebaseSecret, String sLogPath, String sMyName);
void 			SetupWiFi(const char* pcRouterName, const char* pcRouterPW);
Firebase 		SetupFirebase(String acDatabaseURL, String acFirebaseSecret);
void 			SetupHttpServer(const char* acHostname, ESP8266WebServer& oHttpServer, ESP8266HTTPUpdateServer& oHttpUpdateServer);
void 			HandleHttpServer(ESP8266WebServer& oHttpServer);
String 			szLogLineHeader(long lLineCount);
String 			szGetTime(long lMsec);
String 			szAddZeros(int sValue, int sNumDigits);

void 			LogJustToSerial(String sLogline);
void 			LogToSerial(String szLogString);
void 			LogToBoth(Firebase& oFBase, String acPushPath, String szLogString);
//void 			LogToBoth(String acPushPath, String szLogString);
void 			FbaseLogLine(Firebase& oFBase, String acPushPath, String szLogString);
String 			szMakeJSONObject(String szName, String szValue);
String 			szIPaddress(IPAddress oIP);

/*
void		LogToBoth(String szLogString, String szLogValue);
void		LogToBoth(String szLogString, int sLogValue);
void		LogToBoth(String szLogString, float fLogValue);
*/

//Global variables
extern long         	lLineCount;      //Serial Monitor uses for clarity.
extern String			szLogLine;
extern BeckFirebase*	pBeckFBase;

#endif   //BECKLIB_H
