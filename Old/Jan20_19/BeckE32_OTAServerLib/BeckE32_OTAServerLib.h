//BeckE32_OTAServerLib.h, Beck 1/20/19
#pragma once

extern bool               _bOTA_Started;   //Turns off Blynk.
extern unsigned long       _ulUpdateTimeoutMsec;

void SetupOTAServer     (const char *acHostname);
void HandleOTAServer    (void);
//Last line.
