// BeckDisplayLib.cpp 3/4/19a
#include <BeckDisplayLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <Beck_MPU9150.h>
#include <BeckThermoLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>     ////For I2C OLED display

//Declaration for an SSD1306 display connected to I2C
const int       wScreenWidth          = 128;
const int       wScreenHeight         =  64;
const uint8_t   ucDisplayAddress      = 0x3C;
const uint8_t   ucVccState            = SSD1306_SWITCHCAPVCC;

ProjectType         _eDisplayProjectType;

Adafruit_SSD1306    oDisplay(wScreenWidth, wScreenHeight);

void SetupDisplay(ProjectType eDisplayProjectType){
  _eDisplayProjectType= eDisplayProjectType;
  Serial << LOG0 << "SetupDisplay(): Call oDisplay.begin()" << endl;
  //oDisplay.begin        (SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  oDisplay.begin        (ucVccState, ucDisplayAddress);
  oDisplay.clearDisplay ();
  oDisplay.display      ();
  delay(10);
  return;
} //SetupDisplay


void ClearDisplay(){
  oDisplay.clearDisplay ();
  oDisplay.setTextSize  (2);
  oDisplay.setTextColor (WHITE);
  oDisplay.setCursor    (0,0);
  return;
} //ClearDisplay


void UpdateDisplay(){
  switch (_eDisplayProjectType){
    case eFireplace:
    case eGarage:
    case eThermoDev:
    case eHeater:
      UpdateThermDisplay();
      break;
    case eIMU:
      UpdateIMUDisplay();
      break;
    case eFrontLights:
      break;
    default:
      Serial << LOG0 << "BeckDisplayLib.cpp: UpdateDisplay(): Bad switch, _eDisplayProjectType= " << _eDisplayProjectType << endl;
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
  String szDisplayLine1= String(aszAccGyroMagPRY[ePRY][ePitch]);
  String szDisplayLine2= String(aszAccGyroMagPRY[ePRY][eRoll]);
  String szDisplayLine3= String(aszAccGyroMagPRY[ePRY][eYaw]);

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
