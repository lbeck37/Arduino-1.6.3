//BeckESP_OTAWebServerLib.h, Beck 1/25/19
#pragma once
#include <BeckWiFiLib.h>

extern bool                _bOTA_Started;   //Turns off Blynk.
extern unsigned long       _ulUpdateTimeoutMsec;

void SetupOTAServer     (const char *acHostname);
void HandleOTAServer    (void);
//Last line.
