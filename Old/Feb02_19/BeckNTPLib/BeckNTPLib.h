//BeckNTPLib.h, Beck 1/30/19
#pragma once

//#include <NtpClientLib.h>
//#include <NtpClient.h>
//#include <TimeLib.h>
#include <Timezone.h>

extern Timezone         oMT_Timezone;
extern TimeChangeRule*  pTimeChangeRule;

void    SetupNTP        (void);
//Last line.
