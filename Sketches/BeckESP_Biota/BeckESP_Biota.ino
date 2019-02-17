const char szSketchName[]  = "BeckESP_Biota.ino";
const char szFileDate[]    = "Lenny 2/17/19e";
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

#include <BeckAlexaLib.h>
#include <BeckBiotaLib.h>
#include <BeckDisplayLib.h>
#include <BeckI2cLib.h>
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

#include <Streaming.h>
#include <Time.h>
#include <WiFiClient.h>
//#include <Wire.h>

static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";
static const char     szAccessPointSSID[]   = "BiotaSpot";
static const char     szAccessPointPW[]     = "Qazqaz11";

static long           sSystemHandlerSpacing;                //Number of mSec between running system handlers
static unsigned long  ulNextHandlerMsec     = 0;
static int            _wBadCount            = 0;
static int            _wGoodCount           = 0;

#ifdef FRONT_LIGHTS
  //char acBlynkAuthToken[] = "37a58cc7a39045a59bca1fb1281880a2";     //Light Timer Blynk token
  static const char szProjectType[]    = "FRONT_LIGHTS";
  static int wProjectType= sFrontLights;
#endif
#ifdef FIREPLACE
  //char acBlynkAuthToken[] = "35131c5204f34f8e93b574436df46397";
  static const char   acHostname[]    = "BeckFireplace";
  static const char   szProjectType[] = "FIREPLACE";
  static const char   szAlexaName[]   = "Fireplace";
  static int          wProjectType    = sFireplace;
  static const float  fMaxHeatRangeF  = 0.10;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF      = 74;
  static float        _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
#endif
#ifdef GARAGE
  //char acBlynkAuthToken[] = "5e9c5f0ae3f8467597983a6fa9d11101";
  static const char   acHostname[]    = "BeckGarage";
  static const char   szProjectType[] = "GARAGE";
  static int          wProjectType    = sGarage;
  static const float  fMaxHeatRangeF  = 1.00;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF      = 37;
  static float        _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
#endif
#ifdef GARAGE_LOCAL
  //char acBlynkAuthToken[] = "7917cbe7f4614ba19b366a172e629683";
  static const char   acHostname[]    = "BeckGarageLocal";
  static const char   szProjectType[] = "GARAGE_LOCAL";
  static int          wProjectType    = sGarageLocal;
  static const float  fMaxHeatRangeF  = 1.00;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF      = 37;
  static float        _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
#endif
#ifdef HEATER
  //char acBlynkAuthToken[] = "8fe963d2af4e48b5bfb358d91aad583e";
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
  //static const char   acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char   acHostname[]        = "BeckThermoDev";
  static const char   szProjectType[]     = "THERMO_DEV";
  static const int    wProjectType        = sThermoDev;
#endif


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


/*
void SetupI2C(){
  Serial << LOG0 << "SetupI2C(): Call Wire.begin(sSDA_GPIO, sSCL_GPIO)" << endl;
  Wire.begin(sSDA_GPIO, sSCL_GPIO);
  ScanForI2CDevices();
  return;
} //SetupI2C
*/


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


void HandleDevelopment(){
  String szLogString = "HandleDevelopment()";
  LogToSerial(szLogString);
  return;
} //HandleDevelopment


void HandleFrontLights(){
  String szLogString = "HandleFrontLights()";
  LogToSerial(szLogString);
  return;
} //HandleFrontLights


/*
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
*/
//Last line.
