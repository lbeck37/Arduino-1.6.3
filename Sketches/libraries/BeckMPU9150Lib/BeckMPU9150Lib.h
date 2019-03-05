// BeckMPU9150.h 3/5/19a
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

/*
const int       sSDA_GPIO             =   4;   //I2C, GPIO 4 is D2 on NodeMCU
const int       sSCL_GPIO             =   5;   //I2C, GPIO 5 is D1 on NodeMCU and labeled D2
*/

const  int       wBuffChar             = 20;
extern char      aszAccGyroMagPRY  [eLastSensor][eLastAxis][wBuffChar];
extern char      szDegC            [wBuffChar];


//Function protos
void  SetupMPU9150    (const char *szSketchName, const char *szFileDate, uint32_t ulUpdatePeriodMsec);
void  HandleMPU9150       ();
//Last line.
