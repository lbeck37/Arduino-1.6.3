static const char szSketchName[]  = "BeckBlynkESP.ino";
static const char szFileDate[]    = "January 29, 2017C Lenny";

//Uncomment out desired implementation.
//#define FRONT_LIGHTS
//#define FIREPLACE
//#define GARAGE
//#define GARAGE_LOCAL    //Run off local Blynk server.
//#define HEATER
//#define DEV_LOCAL
#define DEV_REMOTE
//#define TANK_MONITOR

#define OTA_SERVER   false     //Skip running OTA server
#include <BeckLib.h>
#include <BeckControlLib.h>
#include <BeckBlynkLib.h>

#if OTA_SERVER
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266HTTPUpdateServer.h>
#endif  //OTA_SERVER

#define LOG0    szLogLineHeader(++lLineCount)
String szLogString;

#ifdef SKIP_BLYNK
  static const bool bSkipBlynk       = true;
#else
  static const bool bSkipBlynk       = false;
#endif

static const int    sFrontLights          = 1;
static const int    sFireplace            = 2;
static const int    sGarage               = 3;
static const int    sGarageLocal          = 4;
static const int    sHeater               = 5;
static const int    sDevLocal             = 6;
static const int    sDevRemote            = 7;
static const int    sTankMonitor          = 8;

//static const char   szRouterName[]        = "Aspot24";
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
#ifdef TANK_MONITOR
  static const char acBlynkAuthToken[]  = "a74c478f829945c2bb5eb9bb88c9f406";
  static const char szProjectType[]     = "TANK_MONITOR";
  static const int  sProjectType        = sTankMonitor;
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
const uint8_t		ucVoltPin= 27;

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
      case sTankMonitor:
        HandleTankMonitor();
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


void HandleTankMonitor(){
  String szLogString = "HandleTankMonitor()";
  LogToBoth(szLogString);
  return;
} //HandleTankMonitor


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
//#ifndef SKIP_SERVER


void HandleHttpServer(void){
  oESP8266WebServer.handleClient();
  delay(1);
  return;
} //HandleHttpServer
#endif	//OTA_SERVER
//Last line.
