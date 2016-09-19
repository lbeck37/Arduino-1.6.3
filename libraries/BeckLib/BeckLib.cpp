//BeckLib.cpp, September 18, 2016
#include <BeckLib.h>
//#define DEBUG_LOGGING
//#define NO_FIREBASE

//Global variables
long					          lLineCount			  = 0;  //Serial Monitor uses for clarity.
int32_t					        wHttpServerCount	= 0;	//Log every nth call at UPLOAD_FILE_WRITE
String					        szLogLine;

bool                    bWiFiOn           = false;
bool                    bFirebaseOn       = false;
bool                    bStartedOTA       = false;
ESP8266WebServer		    oHttpServer(80);
ESP8266HTTPUpdateServer	oHttpUpdateServer(true);

//U8glibs constructor for DOGS102-6 (sometimes called 1701) display
//U8GLIB_DOGS102 u8g(13, 11, 10, 9, 8);     // SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
#ifndef NO_DISPLAY
  U8GLIB_DOGS102 u8g(cSPICLKPin, cSPIMOSIPin, cSPISelectPin, cSPIMISOPin);
#endif

//Digital Pins
#ifdef ESP8266
  //BlynkBeck uses pins 4, 5, 15, 16
  //static const int       sSelectButton;
  //static const int       sBacklightPin;
  const int       sUpButtonPin     =  0;
  const int       sDownButtonPin   =  2;
  const byte      cI2C_SDAPin      =  4;
  const byte      cI2C_SCLPin      =  5;
  const byte      cSPIMISOPin      = 12;
  const byte      cSPIMOSIPin      = 13;
  const byte      cSPICLKPin       = 14;
  const byte      cSPISelectPin    = 15;
  const int       sServoPin        = 16;
#else
  //const int       sSelectButton         = A3;
  const int       sDownButton           = A2;
  const int       sUpButton             = A1;
  const int       sBacklightPin         =  6;
  const int       sServoPin             =  7;
  const byte      cSPICmdDataPin        =  9;
  const byte      cSPIChipSelectPin     = 10;
#endif  //ESP8266


void SetupWiFi(const char* pcRouterName, const char* pcRouterPW){
  wl_status_t eStatus;
	BLogS("SetupWiFi(): Setting WiFi mode to WIFI_AP_STA");
	WiFi.mode(WIFI_AP_STA);

	BLogS("SetupWiFi(): Call WiFi.begin(" + String(pcRouterName) + ", " + String(pcRouterPW) + ")");
	eStatus= WiFi.begin(pcRouterName, pcRouterPW);
	BLogS("SetupWiFi(): WiFi.begin() returned " + szWiFiStatus(eStatus) );

	BLogS("SetupWiFi(): Call WiFi.waitForConnectResult()");
	int wCount= 0;
  int wMaxTries= 3;
	while((WiFi.waitForConnectResult() != WL_CONNECTED) && (++wCount <= wMaxTries)){
	  eStatus= WiFi.status();
	  BLogS("SetupWiFi():W After waitForConnectResult(), WiFi.status= " + szWiFiStatus(WiFi.status()) );
	  BLogS("SetupWiFi():W WiFi failed, retrying.");
	  BLogS("SetupWiFi():W Call WiFi.begin(" + String(pcRouterName) + ", " + String(pcRouterPW) + ") for #" +
	      (String)(wCount + 1) + " time");
	  eStatus= WiFi.begin(pcRouterName, pcRouterPW);
	  BLogS("SetupWiFi():W WiFi.begin() returned " + szWiFiStatus(eStatus) );
	 }
  //BLogS("SetupWiFi(): After WiFi.waitForConnectResult(): " + szWiFiStatus(WiFi.status()));

  eStatus= WiFi.status();
  BLogS("SetupWiFi(): After WiFi.waitForConnectResult(): WiFi status= " + szWiFiStatus(eStatus));
  if (eStatus == WL_CONNECTED) {
    bWiFiOn= true;
  } //if(eStatus==WL_CONNECTED)
  else {
    bWiFiOn= false;
  } //if(eStatus==WL_CONNECTED)else

	BLogS("SetupWiFi(): My WiFi IP address= " + szIPaddress(WiFi.localIP()));
} //SetupWiFi


String szWiFiStatus(wl_status_t status) {
  //wl_status_t defined in wl_definitions.h
  switch(status) {
  case WL_IDLE_STATUS:
    return "WL_IDLE_STATUS";
  case WL_NO_SSID_AVAIL:
    return "WL_NO_SSID_AVAIL";
  case WL_SCAN_COMPLETED:
    return "WL_SCAN_COMPLETED";
  case WL_CONNECTED:
    return "WL_CONNECTED";
  case WL_CONNECT_FAILED:
    return "WL_CONNECT_FAILED";
  case WL_CONNECTION_LOST:
    return "WL_CONNECTION_LOST";
  case WL_DISCONNECTED:
    return "WL_DISCONNECTED";
  case WL_NO_SHIELD:
    return "WL_NO_SHIELD";
  default:
    return "BAD switch value";
  } //switch
} //GetWiFiStatusString


void SetupHttpServer(const char* acHostname,
					ESP8266WebServer& oHttpServer,
					ESP8266HTTPUpdateServer& oHttpUpdateServer){
  BLog("SetupHttpServer(): Call MDNS.begin(" + String(acHostname) + ")");
  MDNS.begin(acHostname);

  BLog("SetupHttpServer(): Call oHttpUpdateServer.setup(&oHttpServer)");
  oHttpUpdateServer.setup(&oHttpServer);

  BLog("SetupHttpServer(): Call oHttpServer.begin())");
  oHttpServer.begin();

  BLog("SetupHttpServer(): Call MDNS.addService(http, tcp, 80)");
  MDNS.addService("http", "tcp", 80);

  BLog("SetupHttpServer(): HTTPUpdateServer ready!");
  if (bWiFiOn) {
    BLog("SetupHttpServer(): Open http://" + String(acHostname) + ".local/update to perform OTA Update.");
  }
  else {
    BLogS("SetupHttpServer(): WiFi is not on, unable to perform OTA Update.");
  }
}	//SetupHttpServer


void HandleHttpServer(ESP8266WebServer& oHttpServer){
  oHttpServer.handleClient();
  delay(1);
} //HandleHttpServer


void LogJustToSerial(String sLogline){
	String sFullLogline=LOG0 + sLogline;
	Serial << sFullLogline << endl;
	return;
}	//LogJustToSerial


String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  szHeader += szGetTime(millis());
  return szHeader;
} //szLogLineHeader


void LogESPValues() {
    BLog("getVcc= " + (String)ESP.getVcc());
    BLog("getFreeHeap= " + (String)ESP.getFreeHeap());
    BLog("getChipId= " + (String)ESP.getChipId());
    BLog("getSdkVersion= " + (String)ESP.getSdkVersion());
    BLog("getBootVersion= " + (String)ESP.getBootVersion());
    BLog("getCpuFreqMHz= " + (String)ESP.getCpuFreqMHz());
    BLog("getFlashChipId= " + (String)ESP.getFlashChipId());
    BLog("getChipId= " + (String)ESP.getChipId());
    BLog("getFlashChipRealSize= " + (String)ESP.getFlashChipRealSize());
    BLog("getFlashChipSize= " + (String)ESP.getFlashChipSize());
    BLog("getFlashChipSpeed= " + (String)ESP.getFlashChipSpeed());
    BLog("getFlashChipSizeByChipId= " + (String)ESP.getFlashChipSizeByChipId());
    BLog("getFreeSketchSpace= " + (String)ESP.getFreeSketchSpace());
    BLog("getSketchSize= " + (String)ESP.getSketchSize());
    return;
}	//LogESPValues


String szGetTime(long lMsec){
  String  szString;
  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";  //Trailing blank.
  return szString;
} //szGetTime


//szAddLeadingZeros() adds 1 or 2 zeros (depending on sNumDigits being 3 or not).
String szAddZeros(int sValue, int sNumDigits){
  String szReturn;
  if ((sNumDigits == 3) && (sValue < 100)){
    szReturn= "0";
  } //if((sNumDigits==3)&&(sValue<100)
  if (sValue < 10){
    szReturn += "0";
  } //if(lValue<10)
  szReturn += String(sValue);
  return szReturn;
} //szAddZeros


String szIPaddress(IPAddress oIP){
  String szDot= ".";
  String szOctetIP= oIP[0] + szDot +  oIP[1] + szDot  + oIP[2] + szDot +  oIP[3];
  return szOctetIP;
} //szIPaddress
//Last line.
