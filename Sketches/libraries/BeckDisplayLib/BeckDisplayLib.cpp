// BeckDisplayLib.cpp 3/5/19b
#include <BeckDisplayLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <BeckMPU9150Lib.h>
#include <BeckThermoLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>     ////For I2C OLED display

//Declaration for an SSD1306 display connected to I2C
const int       wScreenWidth          = 128;
const int       wScreenHeight         =  64;
const uint8_t   ucDisplayAddress      = 0x3C;
const uint8_t   ucVccState            = SSD1306_SWITCHCAPVCC;

const float     fDegToRadians         = PI/180.0;
const uint32_t  ulPrintPeriodMsec     = 5000;

ProjectType         _eDisplayProjectType;

char          aszAccGyroMagPRY  [eLastSensor][eLastAxis][wBuffChar];
char          szDegC            [wBuffChar];

Adafruit_SSD1306    oDisplay(wScreenWidth, wScreenHeight);

//Function protos
void  UpdateThermDisplay      ();
void  UpdatePitchMeterDisplay ();
void  Update4LineDisplay      (String szLine1, String szLine2, String szLine3, String szLine4);
void  BuildDisplayStrings     ();

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
    case ePitchMeter:
      UpdatePitchMeterDisplay();
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


void UpdatePitchMeterDisplay(){
  BuildDisplayStrings();

  String szDisplayLine1= String(aszAccGyroMagPRY[ePRY][ePitch]);
  String szDisplayLine2= String(aszAccGyroMagPRY[ePRY][eRoll]);
  String szDisplayLine3= String(aszAccGyroMagPRY[ePRY][eYaw]);

  Update4LineDisplay(szDisplayLine1, szDisplayLine2, szDisplayLine3, "");
  return;
} //UpdatePitchMeterDisplay


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


void BuildDisplayStrings(){
  //dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf)
  float   fPitchPercent;
  char    szBuffer[wBuffChar];
  for (int eSensor= eAccel; eSensor <= ePRY; eSensor++){
    switch (eSensor){
      case eAccel:
      case eGyro:
      case eMag:
        for (int eAxis= eX; eAxis <= eZ; eAxis++){
          dtostrf(afAccGyroMagPRY[eSensor][eAxis], 5, 2, aszAccGyroMagPRY[eSensor][eAxis]);
        } //for(int eAxis=eX;...
        break;
      case ePRY:
        strcpy(aszAccGyroMagPRY[eSensor][eRoll], "R ");
        dtostrf(afAccGyroMagPRY[eSensor][eRoll], 5, 2, szBuffer);
        strcat(aszAccGyroMagPRY[eSensor][eRoll], szBuffer);

        strcpy(aszAccGyroMagPRY[eSensor][eYaw], "Y ");
        dtostrf(afAccGyroMagPRY[eSensor][eYaw] , 5, 2, szBuffer);
        strcat(aszAccGyroMagPRY[eSensor][eYaw], szBuffer);
        strcat(aszAccGyroMagPRY[eSensor][eYaw], ", ");

        //Compute Pitch as a percent rise or fall
        fPitchPercent= 100.0 * tan(fDegToRadians * afAccGyroMagPRY[ePRY][ePitch]);
        fPitchPercent= fmin(+99.99, fPitchPercent);
        fPitchPercent= fmax(-99.99, fPitchPercent);

        strcpy(aszAccGyroMagPRY[eSensor][ePitch], "P ");
        dtostrf(fPitchPercent, 5, 2, szBuffer);
        strcat(aszAccGyroMagPRY[eSensor][ePitch], szBuffer);
        strcat(aszAccGyroMagPRY[eSensor][ePitch], "%, ");
        break;
      default:
        Serial << LOG0 << "FillSensorData() Bad switch= " << eSensor << endl;
        break;
    } //switch
  } //for(int eSensor=eAccel;...

  //Build temperature string
  dtostrf(fDegC, 4, 1, szDegC);
  strcat(szDegC, "C");
  return;
} //BuildDisplayStrings
//Last Line.
