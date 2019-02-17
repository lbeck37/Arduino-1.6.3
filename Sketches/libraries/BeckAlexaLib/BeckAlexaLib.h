// BeckAlexaLib.h 2/16/19a
#pragma once

#ifndef DO_ALEXA
  #define DO_ALEXA            true
#endif

#if DO_ALEXA
#include <fauxmoESP.h>        //Alexa Phillips Hue light emulation

const char            szAlexaName[]   = "Larry's Device";

extern fauxmoESP      Alexa;                    //Alexa emulation of Phillips Hue Bulb
extern int            wAlexaHandleCount;

void SetupAlexa       (int wProjectType);
void HandleAlexa      ();
void DoAlexaCommand   (unsigned char ucDdeviceID, const char* szDeviceName,
                       bool bState, unsigned char ucValue);

#endif  //DO_ALEXA
//Last line.
