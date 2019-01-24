//BeckE8266OTALib.h, Beck 1/16/19
#pragma once

extern bool               _bOTA_Started;   //Turns off Blynk.
extern unsigned long       _ulUpdateTimeoutMsec;

void SetupOTAServer     (const char *acHostname);
void HandleOTAServer    (void);
//Last line.
