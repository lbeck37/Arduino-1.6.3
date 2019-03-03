const char szSketchName[]  = "MPU9150AHRS";
const char szFileDate[]    = " 03/03/19a";

#include <BeckMiniLib.h>
#include <Beck_MPU9150.h>

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << "," << szFileDate << endl;
  SetupIMUSystem(szSketchName, szFileDate);
  return;
} //setup


void loop(){
  HandleIMU();
  //DisplayData();
  return;
} //loop
//Last line.
