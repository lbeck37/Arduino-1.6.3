#pragma once

#include <ESP8266WiFi.h>

void 		onSTAGotIP(WiFiEventStationModeGotIP ipInfo);
void 		onSTADisconnected(WiFiEventStationModeDisconnected event_info);
String  szFormatTimeString(time_t lCurrentTime);
String 	szFormatDateString(time_t lCurrentTime);
String 	szPrintDigits(int digits);
