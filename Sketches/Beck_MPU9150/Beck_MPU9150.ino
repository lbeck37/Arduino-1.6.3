const char szSketchName[]  = "MPU9150";
const char szFileDate[]    = " 03/05/19b";

#include <BeckDisplayLib.h>
#include <BeckMiniLib.h>
#include "Wire.h"

const uint32_t  ulDisplayPeriodMsec   = 200;

//Function protos
void  SetupI2C    ();

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << "," << szFileDate << endl;
  SetupI2C();
  SetupDisplay(eIMU);
  ClearDisplay();
  SetupIMUSystem(szSketchName, szFileDate, ulDisplayPeriodMsec);
  return;
} //setup

void loop(){
  HandleIMU();
  UpdateDisplay();
  return;
} //loop


void SetupI2C(){
  Serial << LOG0 << "SetupI2C(): Call Wire.begin(sSDA_GPIO= " << sSDA_GPIO << ", sSCL_GPIO= " << sSCL_GPIO << ")" << endl;
  Wire.begin(sSDA_GPIO, sSCL_GPIO);
  return;
} //SetupI2C
//Last line.
