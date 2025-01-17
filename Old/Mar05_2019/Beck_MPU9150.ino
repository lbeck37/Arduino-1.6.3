const char szSketchName[]  = "MPU9150";
const char szFileDate[]    = " 03/05/19a";

#include <BeckDisplayLib.h>
#include <BeckMiniLib.h>
#include <Beck_MPU9150.h>
#include <Adafruit_SSD1306.h>     ////For I2C OLED display
#include "Wire.h"

const uint32_t  ulPrintPeriodMsec     =  5000;
const uint32_t  ulDisplayPeriodMsec   = 200;

uint32_t        ulNextDisplayMsec     = 0;
uint32_t        ulNextPrintMsec       = 0;

//Function protos
//void  DisplayData ();
//void  PrintIMUData();
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


/*
void DisplayData(void){
  if(millis() > ulNextDisplayMsec){
   ulNextDisplayMsec= millis() + ulDisplayPeriodMsec;
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setTextSize(1);
   int    wLine;
   int    wYStart;
   int    wDotsPerLine= 11;

   wLine= 0;
   wYStart= wLine * wDotsPerLine;
   display.setCursor(0, wYStart);
   display.print(aszAccGyroMagPRY[ePRY][ePitch]);
   display.print(aszAccGyroMagPRY[ePRY][eRoll]);

   wLine= 1;
   wYStart= wLine * wDotsPerLine;
   display.setCursor(0, wYStart);
   display.print(aszAccGyroMagPRY[ePRY][eYaw]);
   display.print(szDegC);

   wLine= 2;
   wYStart= wLine * wDotsPerLine;
   display.setCursor(0,  wYStart); display.print(aszAccGyroMagPRY[eAccel][eX]);
   display.setCursor(30, wYStart); display.print(aszAccGyroMagPRY[eAccel][eY]);
   display.setCursor(60, wYStart); display.print(aszAccGyroMagPRY[eAccel][eZ]);

   wLine= 3;
   wYStart= wLine * wDotsPerLine;
   display.setCursor(0,  wYStart); display.print(aszAccGyroMagPRY[eGyro][eX]);
   display.setCursor(30, wYStart); display.print(aszAccGyroMagPRY[eGyro][eY]);
   display.setCursor(60, wYStart); display.print(aszAccGyroMagPRY[eGyro][eZ]);

   wLine= 5;
   wYStart= wLine * wDotsPerLine + 1; //Push to the bottom
   display.setCursor(0, wYStart);
   display.print(szSketchName); display.print(szFileDate);

   display.display();
  } //if(millis()>ulNextDisplayMsec)
  return;
} //DisplayData
*/


/*
void PrintIMUData(){

  if(millis() > ulNextPrintMsec){
    ulNextPrintMsec= millis() + ulPrintPeriodMsec;
    Serial << LOG0 << "HandleIMU(): P R Y " << aszAccGyroMagPRY[ePRY][ePitch] <<
        aszAccGyroMagPRY[ePRY][eRoll] << aszAccGyroMagPRY[ePRY][eYaw] << endl;
    Serial << LOG0 << "HandleIMU(): Pitch= " << pitch << ", Roll= " << roll << ", Yaw= " << yaw <<
        ", average rate= " << (1.0f/fDeltaT) << endl;
    Serial << LOG0 << "HandleIMU():    ax= " << (int)1000*ax << ", ay= " << (int)1000*ay <<
        ", az= " << (int)1000*az << endl;
    Serial << LOG0 << "HandleIMU():    gx= " << gx << ", gy= " << gy << ", gz= " << gz << endl;
    Serial << LOG0 << "HandleIMU():    mx= " << (int)mx << ", my= " << (int)my << ", mz= " << (int)mz << endl;
    //Serial << LOG0 << "HandleIMU():    q0= " << q[0] << ", qx= " << q[1] << ", qy= " << q[2] << ", qz= " << q[3] << endl;
    Serial << LOG0 << "HandleIMU():    Temperature= " << fDegC << " Deg C" << endl;
  } //if(millis()>ulNextPrintMsec)

  if(millis() > ulNextPrintMsec){
    ulNextPrintMsec= millis() + ulPrintPeriodMsec;
  } //if(millis()>ulNextPrintMsec)
  return;
} //PrintIMUData
*/
//Last line.
