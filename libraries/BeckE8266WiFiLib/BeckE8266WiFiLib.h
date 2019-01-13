//BeckE8266WiFiLib.h, 1/13/19
#pragma once
//#include <ESP8266WebServer.h>
#include <IPAddress.h>

/*
extern ESP8266WebServer     *_pSoftAPWebServer;
extern IPAddress             _oAccessPtIPAddress;
*/
/*
IPAddress SetupAccessPoint    ();
void      HandleSoftAPClient  ();
void      SetupWebServer      (IPAddress oIPAddress);
*/
extern IPAddress             _oStationIPAddress;

bool      bSetupWiFi          (const char szRouterName[], const char szRouterPW[]);
void      SetupmDNS           (IPAddress oIPAddress, char* szName);
//Last line.
