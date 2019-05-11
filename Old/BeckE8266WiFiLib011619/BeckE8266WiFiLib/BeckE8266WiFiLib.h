//BeckE8266WiFiLib.h, 1/13/19
#pragma once
#include <IPAddress.h>

extern IPAddress             _oStationIPAddress;

bool      bSetupWiFi          (const char szRouterName[], const char szRouterPW[]);
void      SetupmDNS           (IPAddress oIPAddress, char* szName);
//Last line.
