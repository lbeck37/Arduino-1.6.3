//Beck 12/9/18 BeckOTALib.h
#pragma once
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

extern const char*     			acServerIndex;
extern ESP8266WebServer  		oESP8266WebServer;
extern unsigned long  			ulUpdateTimeoutMsec;
extern bool         				bUpdating;

void SetupOTAServer			(const char *acHostname);
void HandleOTAServer		(void);
void HandleOTAUpdate		(void);
void HandleOTAFileEnd		(HTTPUpload& stHTTPUploadLocal);
void PauseBlynk					(void);
//Last line.
