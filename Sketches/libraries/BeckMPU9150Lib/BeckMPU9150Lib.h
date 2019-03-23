// BeckMPU9150.h 3/5/19b
#pragma once
#include <BeckMiniLib.h>

enum Sensor{
  eAccel = 0,
  eGyro,
  eMag,
  ePRY,
  eLastSensor
};

enum Axis{
  eX = 0,
  eY,
  eZ,
  eLastAxis
};

enum PRY{
  ePitch = 0,
  eRoll,
  eYaw,
  eLastPRY
};

extern float    afAccGyroMagPRY[eLastSensor][eLastAxis];
extern float    fDegC;

//Function protos
int   SetupMPU9150    (const char *szSketchName, const char *szFileDate, uint32_t ulUpdatePeriodMsec);
void  HandleMPU9150   ();
//Last line.
