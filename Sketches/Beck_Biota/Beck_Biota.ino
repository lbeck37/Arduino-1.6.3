const char szSketchName[]  = "Beck_Biota.ino";
const char szFileDate[]    = "Lenny 2/17/19s";

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

static long           sSystemHandlerSpacing = 10 * lMsecPerSec;                //Number of mSec between running system handlers
static unsigned long  ulNextHandlerMsec     = 0;
static int            _wBadCount            = 0;
static int            _wGoodCount           = 0;


void setup(){
  Serial.begin(lSerialMonitorBaud);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  ProjectType   eProjectType= eThermoDev;
  _bSystemOk= SetupSystem(eProjectType);
  if(_bSystemOk){
    SetupWiFi(_acRouterName, _acRouterPW);
    SetupOTAServer(_acHostname);
    #if DO_ACCESS_POINT
      SetupWiFiNameServer(_acAccessPointSSID, _acAccessPointPW);
    #endif  //DO_ACCESS_POINT
    SetupI2C();
    SetupIMU();
    #if DO_NTP
      SetupNTP();
    #endif
    #if DO_ALEXA
      SetupAlexa(_acAlexaName);
    #endif
    SetupDisplay();
    UpdateDisplay();
    SetupSwitches();
    ulLastTaskMsec= millis();
  } //if(_bSystemOk)
  else{
    Serial << LOG0 << "setup(): SetupSystem(): Returned false" << endl;
  } //if(_bSystemOk)else
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
    switch (_eProjectType){
      case eFireplace:
      case eGarage:
      case eThermoDev:
        HandleThermostat();
        CheckTaskTime("HandleSystem(): HandleThermostat()");
        HandleHeatSwitch();
        CheckTaskTime("HandleSystem(): HandleHeatSwitch()");
        HandleIMU();
        UpdateDisplay();
        CheckTaskTime("HandleSystem(): HandleIMU()");
        break;
      case eHeater:
        HandleHeater();
        break;
      case eFrontLights:
        //HandleFrontLights();
        break;
      default:
        Serial << LOG0 << "HandleSystem(): Bad switch, _eProjectType= " << _eProjectType << endl;
        break;
    } //switch
  } //if(millis()>=ulNextHandlerMsec)
  return;
} //HandleSystem
//Last line.
