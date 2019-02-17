const char szSketchName[]  = "BeckESP_Biota.ino";
const char szFileDate[]    = "Lenny 2/16/19f";
//Uncomment out desired implementation.
//#define FRONT_LIGHTS
//#define FIREPLACE
//#define GARAGE
//#define GARAGE_LOCAL    //Run off local Blynk server.
//#define HEATER
//#define DEV_LOCAL
#define THERMO_DEV

#ifndef ESP8266
  #define ESP8266
#endif

#define DO_ALEXA            true
#define DO_NTP              true
#define DO_ACCESS_POINT     true
#define DO_DEBUG            false

#include <BeckAlexaLib.h>
#include <BeckBiotaLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <BeckMPU6050_IMU.h>
#include <BeckSwitchLib.h>
#include <BeckThermoLib.h>
#include <BeckWiFiLib.h>
#if DO_ACCESS_POINT
  #include <BeckE8266AccessPointLib.h>
#endif
#ifdef ESP8266
  #include <BeckESP_OTAWebServerLib.h>
#else
  #include <BeckOTALib.h>   //Beck 1/24/19 not tested
#endif  //ESP8266
#if DO_NTP
  #include <BeckNTPLib.h>
#endif

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>     ////For I2C OLED display
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Streaming.h>
#include <Time.h>
#include <WiFiClient.h>
#include <Wire.h>


static const long     sThermoTimesInRow     = 3;      //Max times temp is outside range before switch

static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";

static const char     szAccessPointSSID[]   = "BiotaSpot";
static const char     szAccessPointPW[]     = "Qazqaz11";

static float          fLastDegF             = 37.88;  //Last temperature reading.
static int            sThermoTimesCount     = 0;      //Number of times temperature out of range
static unsigned long  ulNextHandlerMsec     = 0;
//static unsigned long  ulUpdateTimeoutMsec   = 0;
static bool           bThermoOn             = true;   //Whether thermostat is running.
static bool           bHeatOn               = false;  //If switch is on to turn on Heat.
static long           sSystemHandlerSpacing; //Number of mSec between running system handlers
static int            _wBadCount            = 0;
static int            _wGoodCount           = 0;

#if DO_DEBUG
  static const bool   bDebug                = true;    //Used to select places to disable bDebugLog.
#else
  static const bool   bDebug                = false;   //Used to select places to disable bDebugLog.
#endif  //DO_DEBUG

//To get Blynk Auth Token from the Blynk App, go to the Project Settings (nut icon).
#ifdef FRONT_LIGHTS
  char acBlynkAuthToken[] = "37a58cc7a39045a59bca1fb1281880a2";     //Light Timer Blynk token
  static const char szProjectType[]    = "FRONT_LIGHTS";
  static int wProjectType= sFrontLights;
#endif
#ifdef FIREPLACE
  char acBlynkAuthToken[] = "35131c5204f34f8e93b574436df46397";
  static const char   acHostname[]    = "BeckFireplace";
  static const char   szProjectType[] = "FIREPLACE";
  static const char   szAlexaName[]   = "Fireplace";
  static int          wProjectType    = sFireplace;
  static const float  fMaxHeatRangeF  = 0.10;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF      = 74;
  static float        _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
#endif
#ifdef GARAGE
  char acBlynkAuthToken[] = "5e9c5f0ae3f8467597983a6fa9d11101";
  static const char   acHostname[]    = "BeckGarage";
  static const char   szProjectType[] = "GARAGE";
  static int          wProjectType    = sGarage;
  static const float  fMaxHeatRangeF  = 1.00;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF      = 37;
  static float        _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
#endif
#ifdef GARAGE_LOCAL
  char acBlynkAuthToken[] = "7917cbe7f4614ba19b366a172e629683";
  static const char   acHostname[]    = "BeckGarageLocal";
  static const char   szProjectType[] = "GARAGE_LOCAL";
  static int          wProjectType    = sGarageLocal;
  static const float  fMaxHeatRangeF  = 1.00;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF      = 37;
  static float        _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
#endif
#ifdef HEATER
  char acBlynkAuthToken[] = "8fe963d2af4e48b5bfb358d91aad583e";
  static const char acHostname[]       = "BeckHeater";
  static const char szProjectType[]    = "HEATER";
  static int wProjectType              = sHeater;
#endif
#ifdef DEV_LOCAL
  //static const char acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char acBlynkAuthToken[]  = "9fc34bc2cbb34ddf8d392f7c562fb52e";   //Local server
  static const char acHostname[]        = "BeckDevLocal";
  static const char szProjectType[]     = "DEV_LOCAL";
  static const int  wProjectType        = sDevLocal;
#endif
#ifdef THERMO_DEV
  static const char   acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char   acHostname[]        = "BeckThermoDev";
  static const char   szProjectType[]     = "THERMO_DEV";
  static const int    wProjectType        = sThermoDev;
#endif

//From Adafruit demo ssd1306_128x64_i2c.ino
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oDisplay(SCREEN_WIDTH, SCREEN_HEIGHT);

//Create OneWire instance and tell Dallas Temperature Library to use oneWire Library
OneWire             oOneWire(sOneWireGPIO);
DallasTemperature   oSensors(&oOneWire);

void setup(){
  //sSetupTime();
  Serial.begin(lSerialMonitorBaud);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << "/" << szProjectType << ", " << szFileDate << endl;
  SetupWiFi(szRouterName, szRouterPW);
  SetupOTAServer(acHostname);
#if DO_ACCESS_POINT
  SetupWiFiNameServer(szAccessPointSSID, szAccessPointPW);
#endif  //DO_ACCESS_POINT
  SetupI2C();
  SetupIMU();
#if DO_NTP
  SetupNTP();
#endif
#if DO_ALEXA
  SetupAlexa(wProjectType);
#endif
  SetupDisplay();
  UpdateDisplay();
  SetupSwitches();
  SetupSystem();
  ulLastTaskMsec= millis();
  return;
} //setup


void loop(){
  ulLastTaskMsec= millis();
  HandleOTAServer();
  CheckTaskTime("loop(): HandleOTAServer()");
  HandleNTPUpdate();
  CheckTaskTime("loop(): HandleNTPUpdate()");
#if DO_ACCESS_POINT
  HandleSoftAPClient();       //Listen for HTTP requests from clients
  CheckTaskTime("loop(): HandleSoftAPClient()");
#endif  //DO_ACCESS_POINT

  if (!_bOTA_Started){
    HandleSystem();
    CheckTaskTime("loop(): HandleSystem()");
  } //if(!_bOTA_Started)
  else{
    Serial << LOG0 << "loop(): Check for update timeout" << endl;
    if (millis() > _ulUpdateTimeoutMsec) {
      _bOTA_Started = false;
      Serial << LOG0 << "loop(): Set bUpdating to " << _bOTA_Started << endl;
    } //if(millis()>ulUpdateTimeoutMsec)
  } //if(!_bOTA_Started)else
  return;
} //loop


void SetupDisplay(){
  Serial << LOG0 << "SetupDisplay(): Call oDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)" << endl;
  oDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // Clear the buffer.
  oDisplay.clearDisplay();
  oDisplay.display();
  delay(10);
  return;
} //SetupDisplay


void SetupI2C(){
  Serial << LOG0 << "SetupI2C(): Call Wire.begin(sSDA_GPIO, sSCL_GPIO)" << endl;
  Wire.begin(sSDA_GPIO, sSCL_GPIO);
  ScanForI2CDevices();
  return;
} //SetupI2C


void SetupSystem(){
  Serial << LOG0 << "SetupSystem(): Begin" << endl;
  switch (wProjectType){
  case sThermoDev:
  case sDevLocal:
      sSystemHandlerSpacing = 10 * lMsecPerSec;
      break;
    default:
      sSystemHandlerSpacing = 10 * lMsecPerSec;
      break;
  } //switch
  return;
} //SetupSystem


void HandleSystem(){
#if DO_ALEXA
  HandleAlexa();
  CheckTaskTime("HandleAlexa");
#endif
  if (millis() >= ulNextHandlerMsec){
    _wGoodCount= 0;
    _wBadCount= 0;
    ulNextHandlerMsec= millis() + sSystemHandlerSpacing;
    if (wAlexaHandleCount < 1000){
      //Typically HandleAlexa() gets called ~8,000 times every 10 sec, except when it's 1 or 2
      LogToSerial("HandleSystem():HandleAlexa() Times called=", wAlexaHandleCount);
    } //if (wAlexaHandleCount<1000)
    wAlexaHandleCount= 0;
    switch (wProjectType){
      case sFrontLights:
        HandleFrontLights();
        break;
      case sFireplace:
      case sGarage:
      case sGarageLocal:
      case sThermoDev:
        HandleThermostat();
        CheckTaskTime("HandleSystem(): HandleThermostat()");
        HandleHeatSwitch();
        CheckTaskTime("HandleSystem(): HandleHeatSwitch()");
        HandleIMU();
        UpdateDisplay();
        CheckTaskTime("HandleSystem(): HandleIMU()");
        break;
      case sHeater:
        HandleHeater();
        break;
      case sDevLocal:
        HandleDevelopment();
        break;
      default:
        String szLogString= "HandleSystem():Bad switch";
        LogToSerial(szLogString, wProjectType);
        break;
    } //switch
  } //if(millis()>=ulNextHandlerMsec)
  return;
} //HandleSystem


void UpdateDisplay(void){
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
  //delay(10);
} //UpdateDisplay


void HandleDevelopment(){
  String szLogString = "HandleDevelopment()";
  LogToSerial(szLogString);
  return;
} //HandleDevelopment


void HandleHeater(){
  String szLogString = "HandleHeater()";
  LogToSerial(szLogString);
  return;
} //HandleHeater


void HandleFrontLights(){
  String szLogString = "HandleFrontLights()";
  LogToSerial(szLogString);
  return;
} //HandleFrontLights


void HandleThermostat(){
  unsigned long   ulStartTime;
  ClearTaskTime2(&ulStartTime);
  //Only do something if the thermostat is turned on.
  if (bThermoOn){
    float fDegF= fGetDegF(true);
    CheckTaskTime2("HandleThermostat(): fGetDegF", &ulStartTime);
    if (bHeatOn){
      if (fDegF >= _fThermoOffDegF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnHeatOn(false);
          CheckTaskTime2("HandleThermostat(): TurnHeatOn(false)", &ulStartTime);
          sThermoTimesCount= 0;
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fDegF>=_fThermoOffDegF)
      else{
        sThermoTimesCount= 0;
      } //if(fDegF>=_fThermoOffDegF)else
    } //if(bHeatOn)
    else{
      if (fDegF <= _fSetpointF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnHeatOn(true);
          CheckTaskTime2("HandleThermostat(): TurnHeatOn(true)", &ulStartTime);
          sThermoTimesCount= 0;
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fDegF<_fSetpointF)
      else{
        sThermoTimesCount= 0;
      } //if(fDegF<_fSetpointF)else
    } //if(bHeatOn)else
    LogThermostatData(fDegF);
    CheckTaskTime2("HandleThermostat(): LogThermostatData()", &ulStartTime);
  } //if(bThermoOn)
  else{
    String szLogString= " bThermoOn is false";
    LogToSerial(szLogString);
  }
  return;
} //HandleThermostat


void LogThermostatData(float fDegF){
  String szLogString= " " + String(bHeatOn) + String(sThermoTimesCount) + " " +
                String(fDegF) + " " + String(_fSetpointF) + " " + String(_fThermoOffDegF);
  LogToSerial(szLogString);
  return;
} //LogThermostatData


void DebugHandleBlynkLEDs(){
  String szLogString= "S Now Last: ";
  int sLastSwitch= 4;
  for (int sSwitch= 0; sSwitch <= sLastSwitch; sSwitch++){
    szLogString += sSwitch;
    szLogString += " ";
    szLogString += asSwitchState[sSwitch];
    szLogString += " ";
    szLogString += asSwitchLastState[sSwitch];
    if (sSwitch != sLastSwitch){
      szLogString += ", ";
    } //if(sSwitch!=sLastSwitch)
  } //for
  LogToSerial(szLogString);
  return;
} //DebugHandleBlynkLEDs


void HandleHeatSwitch(){
  if (bHeatOn){
    SetSwitch(sHeatSwitchNum, sOn);
  } //if(bHeatOn)
  else{
    asSwitchState[sHeatSwitchNum]= sOff;
    SetSwitch(sHeatSwitchNum, sOff);
  } //if(bHeatOn)else
  return;
} //HandleHeatSwitch


float fGetDegF(bool bTakeReading){
  float fDegFReturn= 37.99;   //Value used for default in testing w/o reading sensor. fLastDegF
  if (bTakeReading){
    oSensors.requestTemperatures(); // Send the command to get temperatures
    fDegFReturn= oSensors.getTempFByIndex(0);
    fLastDegF= fDegFReturn;
  } //if(bTakeReading)
  else{
    fDegFReturn= fLastDegF;
  } //if(bTakeReading)else
  return fDegFReturn;
}  //fGetDegF


float fRound(float fNum){
  oSensors.requestTemperatures(); // Send the command to get temperatures
  float fRounded= floor(fNum + 0.5);
  return fRounded;
}  //fRound


void TurnHeatOn(bool bTurnOn){
  if (bTurnOn){
    String szLogString= "TurnHeatOn(): ON";
    LogToSerial(szLogString);
    bHeatOn= true;
    SetHeatSwitch(sSwitchClosed);
    sThermoTimesCount= 0;
  } //if(bTurnOn)
  else{
    String szLogString= "TurnHeatOn(): OFF";
    LogToSerial(szLogString);
    bHeatOn= false;
    SetHeatSwitch(sSwitchOpen);
    sThermoTimesCount= 0;
  } //if(bTurnOn)else
  return;
} //TurnHeatOn


void ScanForI2CDevices(void){
  byte ucError, ucAddress;
  int nDevices;
  nDevices = 0;
  for(ucAddress = 1; ucAddress < 127; ucAddress++ ){
    //The i2c_scanner uses the return value of the Write.endTransmisstion to see if
    //a device did acknowledge to the address.
    Wire.beginTransmission(ucAddress);
    ucError = Wire.endTransmission();

    if (ucError == 0){
      Serial << LOG0 << "ScanForI2CDevices(): I2C device found at address 0x";
      if (ucAddress<16){
        Serial.print("0");
      } //if(ucAddress<16)
      Serial.println(ucAddress,HEX);
      nDevices++;
    } //if(ucError==0)
    else if (ucError==4) {
      Serial << LOG0 << "ScanForI2CDevices(): Unknown error at address 0x" << endl;
      if (ucAddress<16) {
        Serial << "0";
      } //if(ucAddress<16)
      Serial.println(ucAddress,HEX);
    } //else if(ucError==4)
  } //for
 if (nDevices == 0){
    Serial << LOG0 << "ScanForI2CDevices(): ***No I2C devices found***" << endl;
 }  //if(nDevices==0)
  return;
} //ScanForDevices
//Last line.
