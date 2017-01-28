static const char szSketchName[]  = "BeckBlynkESP.ino";
static const char szFileDate[]    = "January 28, 2017A Lenny";

//Uncomment out desired implementation.
//#define FRONT_LIGHTS
//#define FIREPLACE
//#define GARAGE
//#define GARAGE_LOCAL    //Run off local Blynk server.
//#define HEATER
//#define DEV_LOCAL
#define DEV_REMOTE
#define OTA_SERVER   false     //Skip running OTA server
#include <BeckLib.h>
#include <BeckControlLib.h>
#include <BeckBlynkLib.h>

#if OTA_SERVER
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266HTTPUpdateServer.h>
#endif  //OTA_SERVER

//#include <BlynkSimpleEsp8266.h>
#include <Time.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#ifndef ESP32
  #include <Adafruit_ADS1015.h>
#endif

#define ONEWIRE_PIN       12

#define LOG0    szLogLineHeader(++lLineCount)
String szLogString;

#ifdef SKIP_BLYNK
  static const bool bSkipBlynk          = true;
#else
  static const bool bSkipBlynk          = false;
#endif
static const int    sSwitchOpen           = 0;
static const int    sSwitchClosed         = 1;
static const int    sOff                  = 0;
static const int    sOn                   = 1;
static const int    sNotInit              = -3737;
static const int    sNumSwitches          = 4;
//static const int    sMaxNumSwitches       = 4;
static const int    sThermoDummySwitch    = 0;  //Thermostat Blynk LED lives at unused switch #0.
static const int    asSwitchPin[]         = {-1, 4, 5, 15, 16};    //0 is not a switch, switches are at 1,2,3,4
static const bool   abSwitchInverted[]    = {0, true, true, true, true};  //Opto-isolated relays close when pulled low.
//(3) types of sketches are supported: front lights, fireplace and garage
static const int    sFrontLights          = 1;
static const int    sFireplace            = 2;
static const int    sGarage               = 3;
static const int    sGarageLocal          = 4;
static const int    sHeater               = 5;
static const int    sDevLocal             = 6;
static const int    sDevRemote            = 7;
static const int    sOneWirePin           = ONEWIRE_PIN;  //Dallas DS18B20 Temperature Sensor
//static const int    sMaxFDelta            = 2;  //Amount room temp can rise above setpoint.

static const int    sFurnaceSwitchNum     = 2;      //Was 1, switch number that turns furnace on and off.
static const long   sThermoTimesInRow     = 3;      //Max times temp is outside range before switch
static const float  fMaxHeatRangeF        = 2.00;   //Temp above setpoint before heat is turned off

//static const char   szRouterName[]        = "Aspot24";
//static const char   szRouterName[]          = "Dspot";
//static const char   szRouterName[]        = "HP7spot";
//static const char   szRouterName[]        = "LenSpot";
//static const char   szRouterName[]        = "P291spot";
static const char   szRouterName[]        = "TPspot";

static const char   szRouterPW[]          = "Qazqaz11";
static const char   acHostname[]          = "esp37";

#ifdef DEBUG
  static const bool       bDebug                = true;    //Used to select places to disable bDebugLog.
#else
  static const bool       bDebug                = false;   //Used to select places to disable bDebugLog.
#endif

//To get Blynk Auth Token from the Blynk App, go to the Project Settings (nut icon).
#ifdef FRONT_LIGHTS
  char acBlynkAuthToken[] = "37a58cc7a39045a59bca1fb1281880a2";     //Light Timer Blynk token
  static const char szProjectType[]    = "FRONT_LIGHTS";
  static int sProjectType= sFrontLights;
#endif
#ifdef FIREPLACE
  char acBlynkAuthToken[] = "35131c5204f34f8e93b574436df46397";
  static const char szProjectType[]    = "FIREPLACE";
  static int sProjectType= sFireplace;
#endif
#ifdef GARAGE
  char acBlynkAuthToken[] = "5e9c5f0ae3f8467597983a6fa9d11101";
  static const char szProjectType[]    = "GARAGE";
  static int sProjectType= sGarage;
#endif
#ifdef GARAGE_LOCAL
  char acBlynkAuthToken[] = "7917cbe7f4614ba19b366a172e629683";
  static const char szProjectType[]    = "GARAGE_LOCAL";
  static int sProjectType= sGarageLocal;
#endif
#ifdef HEATER
  char acBlynkAuthToken[] = "8fe963d2af4e48b5bfb358d91aad583e";
  static const char szProjectType[]    = "HEATER";
  static int sProjectType= sHeater;
#endif
#ifdef DEV_LOCAL
  //static const char acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char acBlynkAuthToken[]    = "9fc34bc2cbb34ddf8d392f7c562fb52e";   //Local server
  static const char szProjectType[]     = "DEV_LOCAL";
  static const int  sProjectType        = sDevLocal;
#endif
#ifdef DEV_REMOTE
  static const char acBlynkAuthToken[]  = "55bce1afbf894b3bb67b7ea34f29d45a";
  static const char szProjectType[]     = "DEV_REMOTE";
  static const int  sProjectType        = sDevRemote;
#endif

//Maxim/Dallas OneWire sensors
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire         oOneWire(sOneWirePin);

/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature   oSensors(&oOneWire);

#ifdef ESP8266
  Adafruit_ADS1115  AtoD(0x48);
#endif

#if OTA_SERVER
  ESP8266WebServer    oESP8266WebServer(80);
#endif

//UpdaterClass    Update; //Declaration at the end of cores\esp8266\Updater.h from BSP

const char*     acServerIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

static unsigned long  ulNextHandlerMsec     = 0;
static unsigned long  ulUpdateTimeoutMsec   = 0;
static long         sSystemHandlerSpacing; //Number of mSec between running system handlers
//static bool         bDebugLog             = true;   //Used to limit number of printouts.
static bool         bUpdating             = false;   //Turns off Blynk.

void setup()
{
  sSetupTime();
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << " setup(): Sketch: " << szSketchName << "/" << szProjectType << ", " << szFileDate << endl;

  //Wire.begin();
  SetupWiFi(szRouterName, szRouterPW);
  SetupBlynk();
  SetupAtoD();
  SetupSwitches();
  SetupSystem();
  return;
} //setup


void loop() {
//#ifndef SKIP_SERVER
#if OTA_SERVER
  HandleHttpServer();
#endif
  //ReadAtoD();
  if (!bSkipBlynk) {
    if (!bUpdating) {
      //Blynk.run();
      RunBlynk();
      HandleSystem();
    } //if(!bUpdating)
    else {
      Serial << LOG0 << " loop(): Check for update timeout, bSkipBlynk= " << bSkipBlynk << endl;
      if (millis() > ulUpdateTimeoutMsec) {
        bUpdating = false;
        Serial << LOG0 << " loop(): Set bUpdating to " << bUpdating
            << endl;
      } //if(millis()>ulUpdateTimeoutMsec)
    } //if(!bUpdating)else
  } //if(!bSkipBlynk)
} //loop


void SetupBlynk(){
/*
  WiFi.mode(WIFI_AP_STA);
  Serial << LOG0 << " SetupServer(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  //wl_status_t eWiFiStatus= WiFi.waitForConnectResult();
  wl_status_t eWiFiStatus= (wl_status_t)WiFi.waitForConnectResult();
  if(eWiFiStatus == WL_CONNECTED) {
    Serial << LOG0 << " SetupServer(): WiFi.waitForConnectResult() returned " << szWiFiStatus(eWiFiStatus) << endl;
    #if OTA_SERVER
      SetupServer();
    #endif
  } //if(eWiFiStatus==WL_CONNECTED)
  else {
    //Serial << LOG0 << " SetupServer(): ERROR: WiFi.waitForConnectResult() returned " << ucWiFiStatus << endl;
    Serial << LOG0 << " SetupServer(): ERROR: WiFi.waitForConnectResult() returned " << szWiFiStatus(eWiFiStatus) << endl;
  } //if(eWiFiStatus==WL_CONNECTED)else
*/

  switch (sProjectType){
    case sGarageLocal:
    case sDevLocal:
      Serial << LOG0 << " SetupBlynk(): Call Blynk.config(" << acBlynkAuthToken << ", IPAddress(192,168,15,191))" << endl;
//      Blynk.config(acBlynkAuthToken, IPAddress(192,168,15,191));
      ConfigBlynk(acBlynkAuthToken, IPAddress(192,168,15,191));
      break;
    default:
      Serial << LOG0 << " SetupBlynk(): Call Blynk.config(" << acBlynkAuthToken << ")" << endl;
//      Blynk.config(acBlynkAuthToken);
      ConfigBlynk(acBlynkAuthToken);
      break;
  } //switch
  Serial << LOG0 << " SetupBlynk(): Blynk.config() returned" << endl;
  return;
} //SetupBlynk


void SetupAtoD(){
#ifdef ESP8266
  szLogString="SetupAtoD(): Call AtoD.begin()";
  LogToBoth(szLogString);
  AtoD.begin();
  szLogString="SetupAtoD(): Call AtoD.begin()";
  LogToBoth(szLogString);
#endif
  return;
} //SetupAtoD


float fReadAtoD(int sChannel){
  float fVoltage= 0.0;
#ifdef ESP8266
  szLogString="fReadAtoD(): Ch=";
  LogToBoth(szLogString, sChannel);
	int sAtoDReading = AtoD.readADC_SingleEnded(sChannel);
  szLogString="fReadAtoD():";
  LogToBoth(szLogString, sAtoDReading);
	//Convert 16bit value from the AtoD into volts
	fVoltage = (sAtoDReading * 0.1875)/1000;
#endif
  return  fVoltage;
} //fReadAtoD


#if OTA_SERVER
void SetupServer(void) {
    MDNS.begin(acHostname);
    oESP8266WebServer.on("/", HTTP_GET, [](){
      oESP8266WebServer.sendHeader("Connection", "close");
      oESP8266WebServer.sendHeader("Access-Control-Allow-Origin", "*");
      oESP8266WebServer.send(200, "text/html", acServerIndex);
    });
    oESP8266WebServer.on("/update", HTTP_POST, []() {
      oESP8266WebServer.sendHeader("Connection", "close");
      oESP8266WebServer.sendHeader("Access-Control-Allow-Origin", "*");
      oESP8266WebServer.send(200, "text/plain", (Update.hasError()) ? "Update Failed!" : "Update Successful!");
      ESP.restart();
    },[](){
      HandleUpdate();
    });
    oESP8266WebServer.begin();
    MDNS.addService("http", "tcp", 80);
    Serial << LOG0 << " SetupServer(): Open http://" << acHostname << ".local to perform an OTA update" << endl;
//#endif
  return;
} //SetupServer


void HandleUpdate() {
  //upload() returns oHttpServer._currentUpload which is an HTTPUpload struct
  HTTPUpload& stHTTPUpload = oESP8266WebServer.upload();
  if (stHTTPUpload.status == UPLOAD_FILE_START) {
    PauseBlynk();
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    uint32_t ulMaxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    Serial << LOG0 << " HandleUpdate(): Update status     = UPLOAD_FILE_START" << endl;
    Serial << LOG0 << " HandleUpdate(): ulMaxSketchSpace  = " << ulMaxSketchSpace << endl;
    Serial << LOG0 << " HandleUpdate(): Update filename   = " << stHTTPUpload.filename << endl;
    Serial << LOG0 << " HandleUpdate(): Update name       = " << stHTTPUpload.name << endl;
    Serial << LOG0 << " HandleUpdate(): Update type       = " << stHTTPUpload.type << endl;
    Serial << LOG0 << " HandleUpdate(): Update totalSize  = " << stHTTPUpload.totalSize << endl;
    Serial << LOG0 << " HandleUpdate(): Update currentSize= " << stHTTPUpload.currentSize << endl;
    if (!Update.begin(ulMaxSketchSpace)) { //start with max available size
      Update.printError(Serial);
    } //if(!Update.begin(maxSketchSpace))
  } //if(WiFi.waitForConnectResult()==WL_CONNECTED)
  else if (stHTTPUpload.status == UPLOAD_FILE_WRITE) {
    //Serial << LOG0 << " Handle /update HTTP_POST: UPLOAD_FILE_WRITE, upload.currentSize= " << stHTTPUpload.currentSize << endl;
    if (Update.write(stHTTPUpload.buf, stHTTPUpload.currentSize)
        != stHTTPUpload.currentSize) {
      Update.printError(Serial);
    } //if(Update.write(upload.buf, upload.currentSize) != upload.currentSize)
  } //else if(upload.status==UPLOAD_FILE_WRITE)
  else if (stHTTPUpload.status == UPLOAD_FILE_END) {
    Serial << LOG0 << " HandleUpdate(): Update status     = UPLOAD_FILE_END" << endl;
    Serial << LOG0 << " HandleUpdate(): Update filename   = " << stHTTPUpload.filename << endl;
    Serial << LOG0 << " HandleUpdate(): Update name       = " << stHTTPUpload.name << endl;
    Serial << LOG0 << " HandleUpdate(): Update type       = " << stHTTPUpload.type << endl;
    Serial << LOG0 << " HandleUpdate(): Update totalSize  = " << stHTTPUpload.totalSize << endl;
    Serial << LOG0 << " HandleUpdate(): Update currentSize= " << stHTTPUpload.currentSize << endl;
    HandleFileEnd(stHTTPUpload);
#if false
    if (Update.end(true)) { //true to set the size to the current progress
      //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      Serial << LOG0
          << " HandleUpdate(): UPLOAD_FILE_END (rebooting?), upload.totalSize= "
          << stHTTPUpload.totalSize << endl;
    } //if(Update.end(true))
    else {
      Update.printError(Serial);
    } //if(Update.end(true))else
    Serial.setDebugOutput(false);
#endif
  } //else if(upload.status==UPLOAD_FILE_END)
  yield();
  return;
} //HandleUpdate


void HandleFileEnd(HTTPUpload& stHTTPUploadLocal) {
  if (Update.end(true)) { //true to set the size to the current progress
    Serial << LOG0 << " HandleFileEnd(): UPLOAD_FILE_END (rebooting?), upload.totalSize= " << stHTTPUploadLocal.totalSize << endl;
  } //if(Update.end(true))
  else {
    Update.printError(Serial);
  } //if(Update.end(true))else
  Serial.setDebugOutput(false);
  return;
} //HandleFileEnd


void PauseBlynk() {
    bUpdating= true;
    Serial << LOG0 << " PauseBlynk(): Set bUpdating to " << bUpdating << endl;
    ulUpdateTimeoutMsec= millis() + 20000;
  return;
} //PauseBlynk
#endif


int sSetupTime(){
  //setTime(0,0,0, 0,0,0);  //hr, min, sec, day, month, year
  return 1;
} //sSetupTime


void SetupSystem(){
  szLogString = "SetupSystem()";
  LogToBoth(szLogString);
  switch (sProjectType){
  case sDevRemote:
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
    pinMode(asSwitchPin[sSwitch], OUTPUT);
    SetSwitch(sSwitch, sSwitchOpen);
  } //for
  return;
} //SetupSwitches


//#ifndef SKIP_SERVER
#if OTA_SERVER
void HandleHttpServer(void){
  oESP8266WebServer.handleClient();
  delay(1);
  return;
} //HandleHttpServer
#endif

void HandleSystem(){
  if (millis() >= ulNextHandlerMsec){
    String szLogString = "HandleSystem()";
    LogToBoth(szLogString);
    ulNextHandlerMsec= millis() + sSystemHandlerSpacing;
    switch (sProjectType){
      case sFrontLights:
        HandleFrontLights();
        break;
      case sFireplace:
        HandleFireplace();
        break;
      case sGarage:
      case sGarageLocal:
        HandleThermostat();
        //HandleBlynkLEDs();
        HandleFurnaceSwitch();
        break;
      case sHeater:
        HandleHeater();
        break;
      case sDevLocal:
      case sDevRemote:
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


void HandleFrontLights(){
  String szLogString = "HandleFrontLights()";
  LogToBoth(szLogString);
  return;
} //HandleFrontLights


void HandleFireplace(){
  String szLogString = "HandleFireplace()";
  LogToBoth(szLogString);
  return;
} //HandleFireplace


void HandleThermostat(){
  String szLogString = "HandleThermostat()";
  LogToBoth(szLogString);
  //Only do anything if the thermostat is turned on.
  if (bThermoOn){
    float fDegF= fGetDegF(true);
    float fRoundDegF= fRound(fDegF);
    DebugHandleThermostat(fDegF);
    if (bFurnaceOn){
      if (fRoundDegF >= fThermoOffDegF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnFurnaceOn(false);
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fRoundDegF>=fThermoOffDegF)
      else{
        sThermoTimesCount= 0;
      } //if(fRoundDegF>=fThermoOffDegF)else
    } //if(bFurnaceOn)
    else{
      if (fRoundDegF <= sSetpointF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnFurnaceOn(true);
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fRoundDegF<sSetpointF)
      else{
        sThermoTimesCount= 0;
      } //if(fRoundDegF<sSetpointF)else
    } //if(bFurnaceOn)else
  } //if(bThermoOn)
  else{
    LogToBoth(szLogString);
    szLogString= " bThermoOn is false";
    LogToBoth(szLogString);
  }
  //} //if(millis()>=ulNextHandlerMsec)
  return;
} //HandleThermostat


void DebugHandleThermostat(float fDegF){
  //String szLogString2= " ";
  String szLogString = "HandleThermostat";
  LogToBoth(szLogString);
  szLogString= " DegF=";
  LogToBoth(szLogString, fDegF);
  szLogString= " SetpointF=";
  LogToBoth(szLogString, sSetpointF);
  szLogString= " OffDegF=";
  LogToBoth(szLogString, fThermoOffDegF);
  szLogString= " bFurnaceOn=";
  LogToBoth(szLogString, bFurnaceOn);
  szLogString= " OnCount=";
  LogToBoth(szLogString, sThermoTimesCount);
  return;
} //DebugHandleThermostat


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




float fRound(float fNum){
  oSensors.requestTemperatures(); // Send the command to get temperatures
  float fRounded= floor(fNum + 0.5);
  return fRounded;
}  //fRound

//Last line.
