// BeckDisplayLib.cpp 2/18/19a
#include <BeckDisplayLib.h>
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


void UpdateDisplay(void){
/*
  oDisplay.clearDisplay();
  oDisplay.setTextSize(2);
  oDisplay.setTextColor(WHITE);
  oDisplay.setCursor(0,0);
  String szDisplayLine= "Now " + String(fLastDegF);
  oDisplay.println(szDisplayLine);

  szDisplayLine= "Set " + String(_fSetpointF);
  oDisplay.println(szDisplayLine);

  szDisplayLine= "Off " + String(_fThermoOffDegF);
  oDisplay.println(szDisplayLine);

  oDisplay.display();
*/

/*
  Serial << LOG0 << "UpdateDisplay(): fLastDegF= " << fLastDegF << endl;
  Serial << LOG0 << "UpdateDisplay(): _fSetpointF= " << _fSetpointF << endl;
  Serial << LOG0 << "UpdateDisplay(): _fThermoOffDegF= " << _fThermoOffDegF << endl;
*/
/*
  char szTempStr[20];
  char szDisplayLine1[20];
  char szDisplayLine2[20];
  char szDisplayLine3[20];
  //char szDisplayLine4[20];
   *
  strcpy(szDisplayLine1, "Now ");
  sprintf(szTempStr, "%5f", fLastDegF);
  strcat(szDisplayLine1, szTempStr);

  strcpy(szDisplayLine2, "Set ");
  sprintf(szTempStr, "%f", _fSetpointF);
  strcat(szDisplayLine2, szTempStr);

  strcpy(szDisplayLine3, "Off ");
  sprintf(szTempStr, "%f", _fThermoOffDegF);
  strcat(szDisplayLine3, szTempStr);
*/
  String szDisplayLine1= "Now " + String(fLastDegF);
  String szDisplayLine2= "Set " + String(_fSetpointF);
  String szDisplayLine3= "Off " + String(_fThermoOffDegF);

  Update4LineDisplay(szDisplayLine1, szDisplayLine2, szDisplayLine3, "Candy Rocks");
  return;
} //UpdateDisplay


//void Update4LineDisplay(char* szLine1, char* szLine2, char* szLine3, char* szLine4){
void Update4LineDisplay(String szLine1, String szLine2, String szLine3, String szLine4){
  //String szTemp= szLine1;
  //Serial << LOG0 << "Update4LineDisplay(): szLine1= " << szTemp << endl;
/*
  Serial << LOG0 << "Update4LineDisplay(): szLine1= " << szLine1 << endl;
  Serial << LOG0 << "Update4LineDisplay(): szLine2= " << szLine2 << endl;
  Serial << LOG0 << "Update4LineDisplay(): szLine3= " << szLine3 << endl;
  Serial << LOG0 << "Update4LineDisplay(): szLine4= " << szLine4 << endl;
*/
  oDisplay.clearDisplay();
  oDisplay.setTextSize(2);
  oDisplay.setTextColor(WHITE);
  oDisplay.setCursor(0,0);

  //String szDisplayLine= "Now " + String(fLastDegF);
  //oDisplay.println(szDisplayLine);
  oDisplay.println(szLine1);

  if(szLine2){
    //szDisplayLine= "Set " + String(_fSetpointF);
    //oDisplay.println(szDisplayLine);
    oDisplay.println(szLine2);
  } //if(szLine2)

  if(szLine3){
    //szDisplayLine= "Off " + String(_fThermoOffDegF);
    //oDisplay.println(szDisplayLine);
    oDisplay.println(szLine3);
  } //if(szLine3)

  if(szLine4){
    //szDisplayLine= "Off " + String(_fThermoOffDegF);
    oDisplay.println(szLine4);
  } //if(szLine4)

  oDisplay.display();
  return;
} //Update4LineDisplay
//Last Line.
