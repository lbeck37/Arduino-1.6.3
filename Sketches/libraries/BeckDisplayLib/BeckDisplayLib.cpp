// BeckDisplayLib.cpp 2/18/19b
#include <BeckDisplayLib.h>
#include <BeckBiotaLib.h>
#include <BeckIMULib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <BeckThermoLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>     ////For I2C OLED display

//From Adafruit demo ssd1306_128x64_i2c.ino
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oDisplay(SCREEN_WIDTH, SCREEN_HEIGHT);

void SetupDisplay(){
  Serial << LOG0 << "SetupDisplay(): Call oDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)" << endl;
  oDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // Clear the buffer.
  oDisplay.clearDisplay();
  oDisplay.display();
  delay(10);
  return;
} //SetupDisplay


void ClearDisplay(){
  oDisplay.clearDisplay();
  oDisplay.setTextSize(2);
  oDisplay.setTextColor(WHITE);
  oDisplay.setCursor(0,0);
  return;
} //ClearDisplay


void UpdateDisplay(){
  switch (_eProjectType){
    case eFireplace:
    case eGarage:
    case eThermoDev:
    case eHeater:
      UpdateThermDisplay();
      break;
    case eSlopeMeter:
      break;
    case eFrontLights:
      break;
    default:
      Serial << LOG0 << "HandleSystem(): Bad switch, _eProjectType= " << _eProjectType << endl;
      break;
  } //switch
  return;
} //UpdateDisplay


void UpdateThermDisplay(){
  String szDisplayLine1= "Now " + String(fLastDegF);
  String szDisplayLine2= "Set " + String(_fSetpointF);
  String szDisplayLine3= "Off " + String(_fThermoOffDegF);

  Update4LineDisplay(szDisplayLine1, szDisplayLine2, szDisplayLine3, "");
  return;
} //UpdateThermDisplay


void UpdateIMUDisplay(){
  String szDisplayLine1= "P% "   + String(_dPitchPercent);
  String szDisplayLine2= "PD "  + String(_dPitchDeg);
  String szDisplayLine3= "Roll "      + String(_dRollDeg);

  Update4LineDisplay(szDisplayLine1, szDisplayLine2, szDisplayLine3, "");
  return;
} //UpdateIMUDisplay


void Update4LineDisplay(String szLine1, String szLine2, String szLine3, String szLine4){
  //Display up to (4) lines of (10) characters
  oDisplay.clearDisplay();
  oDisplay.setTextSize(2);
  oDisplay.setTextColor(WHITE);
  oDisplay.setCursor(0,0);

  oDisplay.println(szLine1);
  oDisplay.println(szLine2);
  oDisplay.println(szLine3);
  oDisplay.println(szLine4);

  oDisplay.display();
  return;
} //Update4LineDisplay
//Last Line.
