const char szSketchName[]  = "BeckESP_Biota.ino";
const char szFileDate[]    = "Lenny 2/17/19h";
//Uncomment out desired implementation.
//#define FRONT_LIGHTS
//#define FIREPLACE
//#define GARAGE
//#define HEATER
//#define THERMO_DEV

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

static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";
static const char     szAccessPointSSID[]   = "BiotaSpot";
static const char     szAccessPointPW[]     = "Qazqaz11";

static long           sSystemHandlerSpacing = 10 * lMsecPerSec;                //Number of mSec between running system handlers
static unsigned long  ulNextHandlerMsec     = 0;
static int            _wBadCount            = 0;
static int            _wGoodCount           = 0;

static char   _acHostname   [50];
static char   _acProjectType[50];
//static int    _wProjectType;

//Function prototypes
void  SetupSystem   (ProjectType eProjectType);

void setup(){
  Serial.begin(lSerialMonitorBaud);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << "/" << _acProjectType << ", " << szFileDate << endl;
  SetupSystem(eThermoDev);
  SetupWiFi(szRouterName, szRouterPW);
  SetupOTAServer(_acHostname);
#if DO_ACCESS_POINT
  SetupWiFiNameServer(szAccessPointSSID, szAccessPointPW);
#endif  //DO_ACCESS_POINT
  SetupI2C();
  SetupIMU();
#if DO_NTP
  SetupNTP();
#endif
#if DO_ALEXA
  SetupAlexa(_eProjectType);
#endif
  SetupDisplay();
  UpdateDisplay();
  SetupSwitches();
  //SetupSystem();
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


void SetupSystem(ProjectType eProjectType){
  Serial << LOG0 << "SetupSystem(): Begin" << endl;
  _eProjectType= eProjectType;
  switch (_eProjectType){
    case eThermoDev:
      strcpy(_acHostname   , "BeckThermoDev");
      strcpy(_acProjectType, "THERMO_DEV");
      strcpy(_acHostname   , "BeckThermoDev");
      break;
    case eNoProject:
    default:
      Serial << LOG0 << "SetupSystem(): Bad switch, _eProjectType= " << _eProjectType << endl;
      break;
  } //switch
  Serial << LOG0 << "SetupSystem(): Project Type set to: " << _acProjectType << endl;
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
        HandleFrontLights();
        break;
      default:
        Serial << LOG0 << "HandleSystem(): Bad switch, _eProjectType= " << _eProjectType << endl;
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
#ifdef HEATER
  //char acBlynkAuthToken[] = "8fe963d2af4e48b5bfb358d91aad583e";
  static const char acHostname[]       = "BeckHeater";
  static const char szProjectType[]    = "HEATER";
  static int wProjectType              = sHeater;
#endif
//Last line.
