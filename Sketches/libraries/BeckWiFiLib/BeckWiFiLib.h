//BeckWiFiLib.h, 3/23/19
#pragma once
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif    //ESP8266

extern bool        _bWiFiConnected;

void    SetupWiFi     (const char szRouterName[], const char szRouterPW[]);
String  szWiFiStatus  (wl_status_t eWiFiStatus);

//Last line.
