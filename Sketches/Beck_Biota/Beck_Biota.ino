const char szSketchName[]  = "Beck_Biota";
const char szFileDate[]    = "3/5/19y";

#ifndef ESP8266
  #define ESP8266
#endif

#define DO_ALEXA            true
#define DO_NTP              false
#define DO_ACCESS_POINT     true

#include <BeckAlexaLib.h>
#include <BeckBiotaLib.h>
#include <BeckDisplayLib.h>
#include <BeckI2cLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <BeckMPU9150Lib.h>
#include <BeckSwitchLib.h>
#include <BeckThermoLib.h>
#include <BeckWiFiLib.h>
#if DO_ACCESS_POINT
  #include <BeckAccessPointLib.h>
#endif
#ifdef ESP8266
  #include <BeckOTAWebServerLib.h>
#else
  #include <BeckOTALib.h>   //Beck 1/24/19 not tested
#endif  //ESP8266
#if DO_NTP
  #include <BeckNTPLib.h>
#endif
#include <Streaming.h>
#include <Time.h>
#include <WiFiClient.h>

static const uint32_t   ulThermHandlerPeriodMsec    = 10 * lMsecPerSec; //mSec between running system handler
static       uint32_t   ulNextThermHandlerMsec      = 0;

static const uint32_t   ulMPU9150HandlerPeriodMsec  = 200;
static const uint32_t   ulMPU9150DisplayPeriodMsec  = ulMPU9150HandlerPeriodMsec;
static       uint32_t   ulNextMPU9150DisplayMsec    = 0;

static int              _wBadCount              = 0;
static int              _wGoodCount             = 0;
static ProjectType      eProjectType            = ePitchMeter;  //Was eThermoDev

void setup(){
  Serial.begin(lSerialMonitorBaud);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  _bSystemOk= SetupSystem(eProjectType);
  if(_bSystemOk){
    SetupWiFi(_acRouterName, _acRouterPW);
    SetupOTAServer(_acHostname);
    #if DO_ACCESS_POINT
      SetupWiFiNameServer(_acAccessPointSSID, _acAccessPointPW);
    #endif  //DO_ACCESS_POINT
    SetupI2C();
    SetupMPU9150(szSketchName, szFileDate, ulMPU9150HandlerPeriodMsec);
    #if DO_NTP
      SetupNTP();
    #endif
    #if DO_ALEXA
      SetupAlexa(_acAlexaName);
    #endif
    SetupDisplay(_eProjectType);
    ClearDisplay();
    SetupSwitches();
    ulLastTaskMsec= millis();
  } //if(_bSystemOk)
  else{
    while(true){
      Serial << LOG0 << "setup(): In infinite loop because SetupSystem(): Returned false" << endl;
      delay(10000); //10 sec
     }  //while(true)
  } //if(_bSystemOk)else
  return;
} //setup


void loop(){
  ulLastTaskMsec= millis();
  HandleOTAServer();
  CheckTaskTime("loop(): HandleOTAServer()");
#if DO_NTP
  HandleNTPUpdate();
#endif
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
  if(_bAlexaChanged){
    _bAlexaChanged= false;
    UpdateDisplay();
  } //if(bAlexaChanged)
#endif
  wAlexaHandleCount= 0;
  switch (_eProjectType){
    case eFireplace:
    case eGarage:
    case eThermoDev:
      if (millis() >= ulNextThermHandlerMsec){
        ulNextThermHandlerMsec= millis() + ulThermHandlerPeriodMsec;
        _wGoodCount= 0;
        _wBadCount= 0;
        if (wAlexaHandleCount < 1000){
          //Typically HandleAlexa() gets called ~8,000 times every 10 sec, except when it's 1 or 2
          LogToSerial("HandleSystem():HandleAlexa() Times called=", wAlexaHandleCount);
        } //if (wAlexaHandleCount<1000)
        ulNextThermHandlerMsec= millis() + ulThermHandlerPeriodMsec;
        HandleThermostat();
        HandleHeatSwitch();
        UpdateDisplay();
      } //if(millis()>=ulNextThermHandlerMsec)
     break;
    case ePitchMeter:
      HandleMPU9150();
      if (millis() >= ulNextMPU9150DisplayMsec){
        ulNextMPU9150DisplayMsec= millis() + ulMPU9150DisplayPeriodMsec;
        UpdateDisplay();
      } //if(millis()>=ulNextMPU9150DisplayMsec)
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
  return;
} //HandleSystem
//Last line.
