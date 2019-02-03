//BeckNTPLib.h, Beck 2/3/19
#pragma once
#include <NTPClient.h>

extern NTPClient    oNTPClient;

void    SetupNTP        (void);
void    HandleNTPUpdate (void);
//Last line.
