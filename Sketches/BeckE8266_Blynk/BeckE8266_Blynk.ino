const char szSketchName[]  = "BeckE8266_Blynk.ino";
const char szFileDate[]    = "Lenny 1/14/19b";

//Uncomment out desired implementation.
//#define FRONT_LIGHTS
//#define FIREPLACE
//#define GARAGE
//#define GARAGE_LOCAL    //Run off local Blynk server.
//#define HEATER
//#define DEV_LOCAL
#define THERMO_DEV

#define DO_BLYNK        true
#define DO_ACCESS_PT    false
#define DO_ALEXA        false

#if 0
  #define DEBUG true
  #define DEBUG_OTA   //Used to skip Blynk code while debugging OTA
#endif

#include <BeckMiniLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BeckE8266WiFiLib.h>
#include <BeckE8266NTPLib.h>
#include <BeckE8266OTALib.h>
#if DO_ACCESS_PT
  #include <BeckE8266AccessPointLib.h>
#endif  //DO_ACCESS_PT
#if DO_BLYNK
  #include <BeckBlynkLib.h>
  #include <BlynkSimpleEsp8266.h>
  #include <Blynk/BlynkTimer.h>
#endif  //DO_BLYNK
#if DO_ALEXA
  #include <BeckE8266AlexaLib.h>
#else
  bool   _bAlexaSwitchOn= false;
#endif  //DO_ALEXA

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
static const int    sHeatSwitchNum        = 1;          //Switch number that turns Heat on and off.
static const int    sAlexaSwitchNum       = sAlexaPin;  //Switch number that Alexa turns on and off.
static const int    sThermoDummySwitch    = 0;          //Thermostat Blynk LED lives at unused switch #0.
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
static const long   lBlynkTimerInterval   = 1000L;

static const long     sThermoTimesInRow     = 3;      //Max times temp is outside range before switch

static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";

static int            asSwitchState[]       = {0, 0, 0, 0, 0};
static int            asSwitchLastState[]   = {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
static float          fLastDegF             = 37.88;  //Last temperature reading.
static int            sThermoTimesCount     = 0;      //Number of times temperature out of range
static unsigned long  ulNextHandlerMsec     = 0;
static bool           bThermoOn             = true;   //Whether thermostat is running.
static bool           bHeatOn               = false;  //If switch is on to turn on Heat.
static long           sSystemHandlerSpacing; //Number of mSec between running system handlers
static bool           bDebugLog             = true;   //Used to limit number of printouts.

#ifdef DEBUG
  static const bool   bDebug                = true;    //Used to select places to disable bDebugLog.
#else
  static const bool   bDebug                = false;   //Used to select places to disable bDebugLog.
#endif

//To get Blynk Auth Token from the Blynk App, go to the Project Settings (nut icon).
#ifdef FRONT_LIGHTS
  char acBlynkAuthToken[] = "37a58cc7a39045a59bca1fb1281880a2";     //Light Timer Blynk token
  static const char szProjectType[]    = "FRONT_LIGHTS";
  static int sProjectType= sFrontLights;
#endif
#ifdef FIREPLACE
  char acBlynkAuthToken[] = "35131c5204f34f8e93b574436df46397";
  static const char   acHostname[]    = "BeckFireplace";
  static const char   szProjectType[] = "FIREPLACE";
  static const char   szAlexaName[]   = "Fireplace";
  static int          sProjectType    = sFireplace;
  static const float  fMaxHeatRangeF  = 0.10;   //Temp above setpoint before heat is turned off
  static float        fSetpointF      = 70.0;
  static float        fThermoOffDegF  = fSetpointF + fMaxHeatRangeF;
 #endif
#ifdef THERMO_DEV
  static const char   acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char   acHostname[]        = "BeckThermoDev";
  static const char   szProjectType[]     = "THERMO_DEV";
  static const char   szAlexaName[]       = "Larry's Device";
  static const int    sProjectType        = sThermoDev;
  static const float  fMaxHeatRangeF      = 1.00;   //Temp above setpoint before heat is turned off
  static float        fSetpointF          = 70;
  static float        fThermoOffDegF      = fSetpointF + fMaxHeatRangeF;
#endif
#ifdef GARAGE
  static const char   acBlynkAuthToken[]  = "5e9c5f0ae3f8467597983a6fa9d11101";
  static const char   acHostname[]        = "BeckGarage";
  static const char   szProjectType[]     = "GARAGE";
  static int          sProjectType        = sGarage;
  static const float  fMaxHeatRangeF      = 1.00;   //Temp above setpoint before heat is turned off
  static float        fSetpointF          = 37;
  static float        fThermoOffDegF      = fSetpointF + fMaxHeatRangeF;
  static const char   szAlexaName[]       = "Garage";
#endif
#ifdef GARAGE_LOCAL
  char acBlynkAuthToken[] = "7917cbe7f4614ba19b366a172e629683";
  static const char   acHostname[]    = "BeckGarageLocal";
  static const char   szProjectType[] = "GARAGE_LOCAL";
  static int          sProjectType    = sGarageLocal;
  static const float  fMaxHeatRangeF  = 1.00;   //Temp above setpoint before heat is turned off
  static float        fSetpointF      = 37;
  static float        fThermoOffDegF  = fSetpointF + fMaxHeatRangeF;
#endif
#ifdef HEATER
  char acBlynkAuthToken[] = "8fe963d2af4e48b5bfb358d91aad583e";
  static const char acHostname[]       = "BeckHeater";
  static const char szProjectType[]    = "HEATER";
  static int sProjectType              = sHeater;
#endif
#ifdef DEV_LOCAL
  //static const char acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char acBlynkAuthToken[]  = "9fc34bc2cbb34ddf8d392f7c562fb52e";   //Local server
  static const char acHostname[]        = "BeckDevLocal";
  static const char szProjectType[]     = "DEV_LOCAL";
  static const int  sProjectType        = sDevLocal;
#endif

//Set up Blynk Widgets
WidgetTerminal      oTerminal(Terminal_V7);
WidgetLCD           LCDWidget(1);
//LED for thermostat state has no actual switch but it will live as unused switch #0.
WidgetLED           oLED0(ThermoLED_V5);
WidgetLED           oLED1(LED_1V13);
WidgetLED           oLED2(LED_2V18);
WidgetLED           oLED3(LED_3V23);
WidgetLED           oLED4(LED_4V28);

//UpdaterClass    Update; //Declaration at the end of cores\esp8266\Updater.h from BSP

//Create objects
Adafruit_SSD1306    oDisplay(-1);   //Looks like -1 is default

//Create OneWire instance and tell Dallas Temperature Library to use oneWire Library
OneWire             oOneWire(sOneWireGPIO);
DallasTemperature   oSensors(&oOneWire);
BlynkTimer          oBlynkTimer;

//Function prototypes
void HandleAccessPoint();
void HandleBlynk();
void HandleSwitches();

void setup(){
  sSetupTime();
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << "setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << "setup(): Sketch: " << szSketchName << "/" << szProjectType << ", " << szFileDate << endl;
  bSetupWiFi(szRouterName, szRouterPW);
#if DO_ACCESS_PT
  SetupAccessPoint();
  SetupWebServer(_oAccessPtIPAddress);
#endif  //DO_ACCESS_PT
  SetupOTAServer(acHostname);
  SetupNTP();
  SetupBlynk();
  SetupAlexa();
  SetupI2C();
  SetupDisplay();
  UpdateDisplay();
  SetupSwitches();
  SetupSystem();
  return;
} //setup


void loop() {
  HandleOTAServer();
  HandleAccessPoint();

  if (!bUpdating) {
    HandleBlynk();
    HandleSystem();
  } //if(!bUpdating)
  else {
    //Serial << LOG0 << "loop(): Check for update timeout, bSkipBlynk= " << bSkipBlynk << endl;
    if (millis() > ulUpdateTimeoutMsec) {
      bUpdating= false;
      Serial << LOG0 << "loop(): Set bUpdating to " << bUpdating << endl;
    } //if(millis()>ulUpdateTimeoutMsec)
  } //if(!bUpdating)else
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
  StartAlexa(szAlexaName);
#endif  //DO_ALEXA
  return;
} //SetupAlexa


void HandleAlexa(){
#if DO_ALEXA
  HandleAlexaLib();
#endif  //DO_ALEXA
  return;
} //HandleAlexa


void BlynkTimerEvent()
{
 //Serial << LOG0 << "BlynkTimerEvent(): Call Blynk.virtualWrite()" << endl;
 Blynk.virtualWrite(ReadF_V0, fLastDegF);
 return;
} //BlynkTimerEvent


void SetupBlynk(void){
#if DO_BLYNK
  switch (sProjectType){
    case sDevLocal:
      Serial << LOG0 << "SetupBlynk(): Call Blynk.config(" << acBlynkAuthToken << ", IPAddress(192,168,15,191))" << endl;
      Blynk.config(acBlynkAuthToken, IPAddress(192,168,15,191));
      break;
    default:
      Serial << LOG0 << "SetupBlynk(): Call Blynk.config(" << acBlynkAuthToken << ")" << endl;
      Blynk.config(acBlynkAuthToken);
      break;
  } //switch
  // Setup a function to be called every second
  //oBlynkTimer.setInterval(1000L, BlynkTimerEvent);
  oBlynkTimer.setInterval(lBlynkTimerInterval, BlynkTimerEvent);
  //Serial << LOG0 << "SetupWiFi(): Blynk.config() returned" << endl;
#endif  //DO_BLYNK
  return;
} //SetupBlynk


void SetupI2C(){
  Serial << LOG0 << "SetupI2C(): Call Wire.begin(sSDA_GPIO, sSCL_GPIO)" << endl;
  Wire.begin(sSDA_GPIO, sSCL_GPIO);
  ScanForI2CDevices();
  return;
} //SetupI2C


int sSetupTime(){
  //setTime(0,0,0, 0,0,0);  //hr, min, sec, day, month, year
  return 1;
} //sSetupTime


void SetupSystem(){
  String szLogString = "SetupSystem()";
  LogToBoth(szLogString);
  switch (sProjectType){
  case sThermoDev:
  case sDevLocal:
      sSystemHandlerSpacing = 10 * lMsecPerSec;
      break;
    default:
      sSystemHandlerSpacing = 10 * lMsecPerSec;
      break;
  } //switch
  //HandleBlynkLEDs();
  return;
} //SetupSystem


void SetupSwitches(){
  String szLogString = "SetupSwitches()";
  LogToBoth(szLogString);
  for (int sSwitch= 1; sSwitch <= sNumSwitches; sSwitch++){
    if(asSwitchPin[sSwitch] != sNoSwitch){
      pinMode(asSwitchPin[sSwitch], OUTPUT);
      SetSwitch(sSwitch, sSwitchOpen);
    } //if(asSwitchPin[sSwitch]!=sNoSwitch)
  } //for
  return;
} //SetupSwitches


void HandleSystem(){
  HandleAlexa();
  if (millis() >= ulNextHandlerMsec){
    ulNextHandlerMsec= millis() + sSystemHandlerSpacing;
    switch (sProjectType){
      case sFrontLights:
        HandleFrontLights();
        break;
      case sFireplace:
      case sGarage:
      case sGarageLocal:
      case sThermoDev:
        HandleThermostat();
        HandleSwitches();
        UpdateDisplay();
        //UpdateBlynk();
        break;
      case sHeater:
        HandleHeater();
        break;
      case sDevLocal:
        HandleDevelopment();
        break;
      default:
        String szLogString= "HandleSystem:Bad switch";
        LogToBoth(szLogString, sProjectType);
        break;
    } //switch
    HandleBlynkLEDs();
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

  szDisplayLine= "Set " + String(fSetpointF);
  oDisplay.println(szDisplayLine);

  szDisplayLine= "Off " + String(fThermoOffDegF);
  oDisplay.println(szDisplayLine);
  oDisplay.display();
  delay(10);
  return;
} //UpdateDisplay

/*
void UpdateBlynk(void){
  Blynk.virtualWrite(ReadF_V0, fLastDegF);
  return;
} //UpdateBlynk
*/

void HandleAccessPoint(){
#if DO_ACCESS_PT
  HandleSoftAPClient();       //Listen for HTTP requests from clients
#endif  //DO_ACCESS_PT
  return;
} //HandleAccessPoint


void HandleBlynk(){
#if DO_BLYNK
  Blynk.run();
  //oBlynkTimer.run();
#endif  //DO_BLYNK
  return;
} //HandleBlynk


void HandleDevelopment(){
  String szLogString = "HandleDevelopment()";
  LogToBoth(szLogString);
  return;
} //HandleDevelopment


void HandleHeater(){
  String szLogString = "HandleHeater()";
  LogToBoth(szLogString);
  return;
} //HandleHeater


void HandleSwitches(){
  if (bHeatOn) {
    SetHeatSwitch(sOn);
  } //if(bHeatOn)
  else {
    SetHeatSwitch(sOff);
  } //if(bHeatOn)else

  if (_bAlexaSwitchOn) {
    SetAlexaSwitch(sOn);
  } //if(_bAlexaSwitchOn)
  else {
    SetAlexaSwitch(sOff);
  } //if(_bAlexaSwitchOn)else
  return;
} //HandleSwitches


void SetHeatSwitch(int sSwitchState){
  SetSwitch(sHeatSwitchNum, sSwitchState);
  return;
} //SetHeatSwitch


void SetAlexaSwitch(int sSwitchState){
  SetSwitch(sAlexaSwitchNum, sSwitchState);
  return;
} //SetAlexaSwitch


void HandleThermostat(){
  //Only do something if the thermostat is turned on.
  if (bThermoOn){
    float fDegF= fGetDegF(true);
    if (bHeatOn){
      if (fDegF >= fThermoOffDegF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnHeatOn(false);
          sThermoTimesCount= 0;
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fDegF>=fThermoOffDegF)
      else{
        sThermoTimesCount= 0;
      } //if(fDegF>=fThermoOffDegF)else
    } //if(bHeatOn)
    else{
      if (fDegF <= fSetpointF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnHeatOn(true);
          sThermoTimesCount= 0;
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fDegF<fSetpointF)
      else{
        sThermoTimesCount= 0;
      } //if(fDegF<fSetpointF)else
    } //if(bHeatOn)else
    LogThermostatData(fDegF);
  } //if(bThermoOn)
  else{
    String szLogString= " bThermoOn is false";
    LogToBoth(szLogString);
  }
  return;
} //HandleThermostat


void LogThermostatData(float fDegF){
  String szLogString= String(bHeatOn) + String(sThermoTimesCount) + " " +
                String(fDegF) + " " + String(fSetpointF) + " " + String(fThermoOffDegF);
  LogToBoth(szLogString);
  return;
} //LogThermostatData


void HandleFrontLights(){
  String szLogString = "HandleFrontLights()";
  LogToBoth(szLogString);
  return;
} //HandleFrontLights


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
  LogToBoth(szLogString);
  return;
} //DebugHandleBlynkLEDs


void HandleBlynkLEDs(){
  String szLogString = "HandleBlynkLEDs()";
  //DebugHandleBlynkLEDs();
  //Only send data back to Blynk if state of LED has changed.
  //static int asSwitchLastState[]= {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
  bDebugLog= bDebug;  //If bDebug is OfF then we turn logging off for this routine, resetting at end.
  for (int sSwitch= 0; sSwitch <= sNumSwitches; sSwitch++){
    if (true || (asSwitchState[sSwitch] != asSwitchLastState[sSwitch])){
      asSwitchLastState[sSwitch]= asSwitchState[sSwitch];
      switch (sSwitch){
        case 0:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED0.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED0.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 1:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED1.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch ON LED";
            oLED1.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 2:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED2.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED2.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 3:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED3.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED3.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 4:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED4.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED4.off();
          } //if(asSwitchState[sSwitch])else
          break;
        default:
          break;
      } //switch
      //LogToBoth(szLogString, sSwitch);
    } //if(asSwitchState[sSwitch]!=asSwitchLastState[sSwitch])
  } //for
  bDebugLog= true;
  return;
} //HandleBlynkLEDs


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
    LogToBoth(szLogString);
    bHeatOn= true;
    SetHeatSwitch(sSwitchClosed);
    sThermoTimesCount= 0;
  } //if(bTurnOn)
  else{
    String szLogString= "TurnHeatOn(): OFF";
    LogToBoth(szLogString);
    bHeatOn= false;
    SetHeatSwitch(sSwitchOpen);
    sThermoTimesCount= 0;
  } //if(bTurnOn)else
  return;
} //TurnHeatOn


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
  //LogToBoth(szLogString);

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
  for(ucAddress = 1; ucAddress < 127; ucAddress++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(ucAddress);
    ucError = Wire.endTransmission();

    if (ucError == 0){
      Serial << LOG0 << "ScanForI2CDevices(): I2C device found at address 0x";
      if (ucAddress<16){
        Serial.print("0");
      } //if(ucAddress<16)
      Serial.println(ucAddress,HEX);
      //Serial.println("  !");
      nDevices++;
    } //if(ucError==0)
    else if (ucError==4) {
      //Serial.print("Unknown error at address 0x");
      Serial << LOG0 << "ScanForI2CDevices(): Unknown error at address 0x";
      if (ucAddress<16) {
        Serial.print("0");
      } //if(ucAddress<16)
      Serial.println(ucAddress,HEX);
    } //else if(ucError==4)
  }
 if (nDevices == 0){
    Serial.println("No I2C devices found\n");
 }  //if(nDevices==0)
  return;
} //ScanForDevices


// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
//int WriteTerminalLine(char *szString){
void WriteTerminalLine(String szString){
  if (bDebugLog){
    oTerminal.println(szString) ;
    oTerminal.flush();          // Ensure everything is sent
  } //if(bDebugLog)
  return;
} //WriteTerminalLine


void WriteTerminalString(String szString){
  if (bDebugLog){
    oTerminal.print(szString);
    oTerminal.print(" ");       //Send training space
    oTerminal.flush();          // Ensure everything is sent
  } //if(bDebugLog)
  return;
} //WriteTerminalString


//LogToBoth() and BlynkLogLine()have multiple versions
//depending on there being a 2nd variable and its type.
void LogToBoth(String szLogString){
  Serial << LOG0 << szLogString << endl;
  BlynkLogLine(szLogString);
  return;
} //LogToBoth:empty


void LogToBoth(String szLogString, String szLogValue){
  Serial << LOG0 << szLogString << " " << szLogValue << endl;
  BlynkLogLine(szLogString, szLogValue);
  return;
} //LogToBoth:String


void LogToBoth(String szLogString, int sLogValue){
  Serial << LOG0 << szLogString << " " << sLogValue << endl;
  BlynkLogLine(szLogString, sLogValue);
  return;
} //LogToBoth:int


void LogToBoth(String szLogString, float fLogValue){
  Serial << LOG0 << szLogString << " " << fLogValue << endl;
  BlynkLogLine(szLogString, fLogValue);
  return;
} //LogToBoth:float


void BlynkLogLine(String szString){
  String szTermString= szLogLineHeader();
  szTermString += szString;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:empty


void BlynkLogLine(String szString, String szLogValue){
  String szTermString= szLogLineHeader();
  szTermString += szString;
  szTermString +=  " ";
  szTermString +=  szLogValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:String


void BlynkLogLine(String szString, int sValue){
  String szTermString= szLogLineHeader();
  szTermString += szString;
  szTermString +=  " ";
  szTermString +=  sValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:int


void BlynkLogLine(String szString, float fValue){
  String szTermString= szLogLineHeader();
  szTermString += szString;
  szTermString +=  fValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:float


void SendIntToBlynk(int sVirtualPin, int sValue){
  String szString= " SendIntToBlynk: ";
  BlynkLogLine(szString, sValue);
  Blynk.virtualWrite(sVirtualPin, sValue);
  return;
} //SendIntToBlynk


//BLYNK_READ() functions are called by the Blynk app on the phone (at a 1 second interval)
//and returns the value or state of some variable.
//BLYNK_WRITE() functions are called by the Blynk app on the phone
//and pass a variable in the "param" object.
BLYNK_READ(ReadF_V0){
  Blynk.virtualWrite(ReadF_V0, fLastDegF);
} //BLYNK_READ(ReadF_V0)


BLYNK_READ(ReadF_V1){
  Blynk.virtualWrite(ReadF_V1, fLastDegF);
} //BLYNK_READ(ReadF_V1)


BLYNK_WRITE(SetSetpointF_V2){
  int sSetpointParam= param.asInt();
  fSetpointF= sSetpointParam;
  fThermoOffDegF= fSetpointF + fMaxHeatRangeF;
  String szLogString= "SetSetpointF_V2 ";
  LogToBoth(szLogString, fSetpointF);

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  Blynk.virtualWrite(GetSetpointF_V3, fSetpointF);
} //BLYNK_WRITE(Switch_2V15)


BLYNK_READ(GetSetpointF_V3){
  float fReturnF= fSetpointF;
  String szLogString= "GetSetpointF_V3 ";
  LogToBoth(szLogString, fReturnF);

  Blynk.virtualWrite(GetSetpointF_V3, fReturnF);
} //BLYNK_READ(GetSetpointF_V3)


BLYNK_WRITE(ThermoSwitch_V4){
  //Turn thermostat on and off.
  int sParam= param.asInt();
  String szLogString= "ThermoSwitch_V4 ";
  LogToBoth(szLogString, sParam);
  //SetThermoState(sParam);
  //HandleSwitches();
  TurnHeatOn((bool)sParam);

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  Blynk.virtualWrite(GetSetpointF_V3, fSetpointF);
  return;
} //BLYNK_WRITE(ThermoSwitch_V4)

//WidgetLED oLED0(ThermoLED_V5) is constructed earlier

BLYNK_READ(AtoD_1V6){
  static float fVolts= 1.0;
  fVolts= fVolts + 0.001;
  String szLogString= "Read AtoD_1V6 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_1V6, fVolts);
} //BLYNK_READ(AtoD_1V6)


//Handler callback function called when Button set as a Switch is pressed.
//Light around button is lit when 1 is passed as parameter, unlit when 0 is passed.
//Opto-isolated relay is inverse logic, pulling input pin low cn relay.
//Relay #0 is connected to Blynk virtual pins 10, 11, 12
//Relay #1 is connected to Blynk virtual pins 20, 21, 22
BLYNK_WRITE(Switch_1V10){
  //Virtual pin 10, 15, 20 and 25 control Switches 1, 2, 3 and 4.
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();

  String szLogString= "Set Switch_1V10 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_1V10)


BLYNK_WRITE(TimerA_1V11){
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_1V11 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
    }
  else{
    sSwitchSetting= sSwitchOpen;
    }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
  } //BLYNK_WRITE(TimerA_1V11)


BLYNK_WRITE(TimerB_1V12){
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_1V12 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
    }
  else{
    sSwitchSetting= sSwitchOpen;
    }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
  } //BLYNK_WRITE(TimerB_1V12)


//WidgetLED oLED1(LED_1V13) is constructed earlier


BLYNK_READ(AtoD_2V14){
  static float fVolts= 2.0;
  fVolts= fVolts + 0.001;
  String szLogString= "Read AtoD_2V14 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_2V14, fVolts);
} //BLYNK_READ(AtoD_2V14)


BLYNK_WRITE(Switch_2V15){
  int sSwitchNumber= 2;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set Switch2V15 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_2V15)


BLYNK_WRITE(TimerA_2V16){
  int sSwitchNumber= 2;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_2V16 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerA_2V16)


BLYNK_WRITE(TimerB_2V17){
  int sSwitchNumber= 2;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_2V17 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerB_2V17)


//WidgetLED oLED1(LED_2V18) is constructed earlier


BLYNK_READ(AtoD_3V19){
  static float fVolts= 3.0;
  fVolts= fVolts + 0.001;
  String szLogString= "Read AtoD_3V19 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_3V19, fVolts);
} //BLYNK_READ(AtoD_3V19)


BLYNK_WRITE(Switch_3V20){
  int sSwitchNumber= 3;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set Switch_3V20 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_3V20)


BLYNK_WRITE(TimerA_3V21){
  int sSwitchNumber= 3;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_3V21 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerA_3V21)


BLYNK_WRITE(TimerB_3V22){
  int sSwitchNumber= 3;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_3V22 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerB_3V22)


//WidgetLED oLED1(LED_3V23) is constructed earlier


BLYNK_READ(AtoD_4V24){
  static float fVolts= 4.0;
  fVolts= fVolts + 0.001;
  String szLogString= "Read AtoD_4V24 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_4V24, fVolts);
} //BLYNK_READ(AtoD_4V24)


BLYNK_WRITE(Switch_4V25){
  int sSwitchNumber= 4;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set Switch_4V25 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_4V25)


BLYNK_WRITE(TimerA_4V26){
  int sSwitchNumber= 4;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_4V26 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerA_4V26)


BLYNK_WRITE(TimerB_4V27){
  int sSwitchNumber= 4;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_4V27 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerB_4V27)

//WidgetLED oLED1(LED_4V28) is constructed earlier

//Last line.
