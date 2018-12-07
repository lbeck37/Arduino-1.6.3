#pragma once

#include <BeckMiniLib.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP8266WiFi.h>

void onSTAGotIP(WiFiEventStationModeGotIP ipInfo);
void onSTADisconnected(WiFiEventStationModeDisconnected event_info);