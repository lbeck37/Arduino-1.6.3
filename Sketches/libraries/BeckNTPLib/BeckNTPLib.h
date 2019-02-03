//BeckNTPLib.h, Beck 2/2/19
#pragma once
#include <NTPClient.h>

extern NTPClient     oNTPClient;

void    SetupNTP        (void);
void    HandleNTPUpdate (void);

//Last line.
