const char szSketchName[]  = "BeckESP_Biota.ino";
const char szFileDate[]    = "Lenny 2/15/19t";
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
//#define DO_BLYNK            false
#define DO_ALEXA            true
#define DO_NTP              true
#define DO_ACCESS_POINT     true
#define DO_DEBUG            false

#include <BeckMiniLib.h>
#include <BeckWiFiLib.h>
#ifdef ESP8266
  #include <BeckESP_OTAWebServerLib.h>
#else
  #include <BeckOTALib.h>   //Beck 1/24/19 not tested
#endif  //ESP8266
#if DO_ACCESS_POINT
  #include <BeckE8266AccessPointLib.h>
#endif
#if DO_NTP
  #include <BeckNTPLib.h>
#endif
#if DO_ALEXA
  #include <fauxmoESP.h>        //Alexa Phillips Hue light emulation
#endif
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Streaming.h>
#include <Time.h>

//For I2C OLED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static const int  sAlexaPin       =  2;   //GPIO 2 is D4 and Blue LED on NodeMCU
static const int  sSDA_GPIO       =  4;   //I2C, GPIO 4 is D2 on NodeMCU
static const int  sSCL_GPIO       =  5;   //I2C, GPIO 5 is D1 on NodeMCU and labeled D2

static const int  sOneWireGPIO    = 12;   //GPIO 12 is D6 on NodeMCU
static const int  sHeatSwitchGPIO = 14;   //GPIO 14 is D5 on NodeMCU

static const int    sSwitchOpen           = 0;
static const int    sSwitchClosed         = 1;
static const int    sOff                  = 0;
static const int    sOn                   = 1;
static const int    sNotInit              = -3737;
static const int    sNoSwitch             = -1;
static const int    sNumSwitches          = 2;
static const int    sHeatSwitchNum        = 1;      //Switch number that turns Heat on and off.
static const int    sAlexaSwitchNum       = 2;      //Switch number that Alexa turns on and off.
static const int    sThermoDummySwitch    = 0;  //Thermostat Blynk LED lives at unused switch #0.
static const int    asSwitchPin[]         = {-1, sHeatSwitchGPIO, sAlexaPin, sNoSwitch, sNoSwitch};    //0 is not a switch, switches are at 1,2,3,4
static const bool   abSwitchInverted[]    = {0, true, true, true, true};  //Opto-isolated relays close when pulled low.
//(3) types of sketches are supported: front lights, fireplace and garage
static const int    sFrontLights          = 1;
static const int    sFireplace            = 2;
static const int    sGarage               = 3;
static const int    sGarageLocal          = 4;
static const int    sHeater               = 5;
static const int    sDevLocal             = 6;
static const int    sThermoDev            = 7;

static const long     sThermoTimesInRow     = 3;      //Max times temp is outside range before switch

static const char     szRouterName[]        = "Aspot24";
//static const char     szRouterName[]        = "Dspot";    //Verizon hotspot on Larry's Note9
static const char     szRouterPW[]          = "Qazqaz11";

static const char     szAccessPointSSID[]   = "BiotaSpot";
static const char     szAccessPointPW[]     = "Qazqaz11";

static int            asSwitchState[]       = {0, 0, 0, 0, 0};
static int            asSwitchLastState[]   = {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
static float          fLastDegF             = 37.88;  //Last temperature reading.
static int            sThermoTimesCount     = 0;      //Number of times temperature out of range
static unsigned long  ulNextHandlerMsec     = 0;
//static unsigned long  ulUpdateTimeoutMsec   = 0;
static bool           bThermoOn             = true;   //Whether thermostat is running.
static bool           bHeatOn               = false;  //If switch is on to turn on Heat.
static bool           bAlexaOn              = false;  //Only projects that use Alexa set this true.
static long           sSystemHandlerSpacing; //Number of mSec between running system handlers
static bool           bDebugLog             = true;   //Used to limit number of printouts.
static int            wAlexaHandleCount     = 0;      //Incremented each time HandleAlexa() called
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
#if DO_ALEXA
  fauxmoESP           Alexa;                    //Alexa emulation of Phillips Hue Bulb
#endif
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
  static const char   szAlexaName[]       = "Larry's Device";
  //static const char   szAlexaName[]       = "Larry's Biota";
  static const int    wProjectType        = sThermoDev;
  static const float  fMaxHeatRangeF      = 0.10;   //Temp above setpoint before heat is turned off
  static float        _fSetpointF         = 70.0;
  static float        _fThermoOffDegF     = _fSetpointF + fMaxHeatRangeF;
#if DO_ALEXA
  fauxmoESP           Alexa;                    //Alexa emulation of Phillips Hue Bulb
#endif
#endif

//Create objects
//From Adafruit demo ssd1306_128x64_i2c.ino
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 oDisplay(SCREEN_WIDTH, SCREEN_HEIGHT);

//Create OneWire instance and tell Dallas Temperature Library to use oneWire Library
OneWire             oOneWire(sOneWireGPIO);
DallasTemperature   oSensors(&oOneWire);

//Function prototypes
float   fSetThermoSetpoint    (int wSetpoint);
float   fSetThermoSetpoint    (float fSetpoint);
void    LogToSerial           (String szLogString);
void    LogToSerial           (String szLogString, String szLogValue);
void    LogToSerial           (String szLogString, int sLogValue);
void    LogToSerial           (String szLogString, float fLogValue);


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
  //SetupBlynk();
  SetupI2C();
#if DO_NTP
  SetupNTP();
#endif
  SetupAlexa();
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


void SetupAlexa(){
#if DO_ALEXA
  String szLogString= "SetupAlexa(): Begin";
  LogToSerial(szLogString);
  switch (wProjectType){
    case sFireplace:
    case sThermoDev:
      //Only these projects use Alexa
      bAlexaOn= true;
      break;
    default:
      bAlexaOn= false;
      break;
  } //switch
  if(bAlexaOn){
    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    //Beck 12/3/18 I don't know why we do enable, disable, enable but it is necessary
    Alexa.enable(true);
    Alexa.enable(false);
    Alexa.enable(true);

    // You can use different ways to invoke Alexa to modify the devices state:
    // "Alexa, turn light one on" ("light one" is the name of the first device below)
    // "Alexa, turn on light one"
    // "Alexa, set light one to fifty" (50 means 50% of brightness)

    // Add virtual devices
    Alexa.addDevice(szAlexaName);

    //Define the callback
    Alexa.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value)
      {
      DoAlexaCommand(device_id, device_name, state, value);
      } );  //Alexa.onSetState
  } //if(bAlexaOn)
  else{
    szLogString = "SetupAlexa(): Alexa is not enabled for project ";
    LogToSerial(szLogString, wProjectType);
  } //if(bAlexaOn)else
#endif  //DO_ALEXA
  return;
} //SetupAlexa


void HandleAlexa(){
#if DO_ALEXA
  wAlexaHandleCount++;  //Track how many times this is called before next handle system (10 sec)
  Alexa.handle();
#endif  //DO_ALEXA
  return;
} //HandleAlexa


#if DO_ALEXA
void DoAlexaCommand(unsigned char ucDdeviceID, const char* szDeviceName, bool bState, unsigned char ucValue){
  char    szCharString[100];
  sprintf(szCharString, " DoAlexaCommand(): Device #%d (%s) bState: %s value: %d",
      ucDdeviceID, szDeviceName, (bState ? "ON " : "OFF"), ucValue);
  String szLogString= szCharString;
  LogToSerial(szLogString);
  SetAlexaSwitch(bState);
  fSetThermoSetpoint((int)ucValue);
  return;
} //DoAlexaCommand
#endif  //DO_ALEXA


float fSetThermoSetpoint(int wSetpoint){
  float fSetpoint= round( ((float)wSetpoint / 255.0) * 100.0);
  fSetThermoSetpoint(fSetpoint);
  return fSetpoint;
} //fSetThermoSetpoint


float fSetThermoSetpoint(float fSetpoint){
  if(fSetpoint != _fSetpointF){
    _fSetpointF      = fSetpoint;
    _fThermoOffDegF   = _fSetpointF + fMaxHeatRangeF;
    Serial << LOG0 << "fSetThermoSetpoint(): Set _fSetpointF to " << _fSetpointF << endl;
  } //if(fSetpoint!=_fSetpointF)
  return _fSetpointF;
} //fSetThermoSetpoint


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


void SetupSwitches(){
  Serial << LOG0 << "SetupSwitches(): Begin" << endl;
  for (int sSwitch= 1; sSwitch <= sNumSwitches; sSwitch++){
    if(asSwitchPin[sSwitch] != sNoSwitch){
      pinMode(asSwitchPin[sSwitch], OUTPUT);
      SetSwitch(sSwitch, sSwitchOpen);
    } //if(asSwitchPin[sSwitch]!=sNoSwitch)
  } //for
  return;
} //SetupSwitches


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
        UpdateDisplay();
        CheckTaskTime("HandleSystem(): UpdateDisplay()");
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
    asSwitchState[sHeatSwitchNum]= sOff;    SetSwitch(sHeatSwitchNum, sOff);
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


void SetHeatSwitch(int sSwitchState){
  SetSwitch(sHeatSwitchNum, sSwitchState);
  return;
} //SetHeatSwitch


void SetAlexaSwitch(int sSwitchState){
  SetSwitch(sAlexaSwitchNum, sSwitchState);
  return;
} //SetAlexaSwitch


void SetSwitch(int sSwitch, int sSwitchState){
  int sSwitchPin= asSwitchPin[sSwitch];
  bool bPinSetting;
  asSwitchState[sSwitch]= sSwitchState;
  bDebugLog= bDebug;
  if (abSwitchInverted[sSwitch]){
    bPinSetting= !sSwitchState;
  } //if(abSwitchInverted[sSwitch])
  else{
    bPinSetting= sSwitchState;
  } //if(abSwitchInverted[sSwitch])else
  String szLogString= "SetSwitch:  ";
  szLogString += sSwitch;
  szLogString += ",";
  szLogString += sSwitchState;
  szLogString += ",";
  szLogString += sSwitchPin;
  szLogString += ",";
  szLogString += bPinSetting;
  //LogToSerial(szLogString);

  //Some switch positions don't have pins, just Blynk LEDs.
  if (sSwitchPin >= 0){
    digitalWrite(sSwitchPin, bPinSetting);
    asSwitchState[sSwitch]= sSwitchState;
  } //if(sSwitchPin>=0)
  bDebugLog= true;
  return;
} //SetSwitch


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


//LogToSerial() has multiple versions depending on there being a 2nd variable and its type.
void LogToSerial(String szLogString){
  String szTermString= szLogLineHeader();
  szTermString += szLogString;
  Serial << szTermString << endl;
  return;
} //LogToSerial:empty


void LogToSerial(String szLogString, String szLogValue){
  Serial << LOG0 << szLogString << " " << szLogValue << endl;
  return;
} //LogToSerial:String


void LogToSerial(String szLogString, int sLogValue){
  Serial << LOG0 << szLogString << " " << sLogValue << endl;
  return;
} //LogToSerial:int


void LogToSerial(String szLogString, float fLogValue){
  Serial << LOG0 << szLogString << " " << fLogValue << endl;
  return;
} //LogToSerial:float
//Last line.
