//BeckOTAWebServerLib.h, Beck 2/18/19a
#pragma once
#include <BeckWiFiLib.h>

extern bool                _bOTA_Started;   //Turns off Blynk.
extern unsigned long       _ulUpdateTimeoutMsec;

void SetupOTAServer     (const char *acHostname);
void HandleOTAServer    (void);
//Last line.
