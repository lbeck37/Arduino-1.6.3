// BeckAlexaLib.h 12/19/19a
#pragma once

/*
#ifndef DO_ALEXA
  #define DO_ALEXA            true
#endif
*/

#if DO_ALEXA
#include <fauxmoESP.h>        //Alexa Phillips Hue light emulation

extern char           _acAlexaName[];

extern fauxmoESP      Alexa;                    //Alexa emulation of Phillips Hue Bulb
extern int            wAlexaHandleCount;
extern bool           _bAlexaChanged;

void SetupAlexa             (char acAlexaName[]);
void HandleAlexa            ();
void DoAlexaCommand         (unsigned char ucDdeviceID, const char* szDeviceName,
                             bool bState, unsigned char ucValue);

#endif  //DO_ALEXA
//Last line.
