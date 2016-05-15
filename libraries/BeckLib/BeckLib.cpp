//BeckLib.cpp, May 15, 2016
#include <BeckLib.h>
//#define DEBUG_LOGGING
//#define NO_FIREBASE

//Global variables
long					          lLineCount			  = 0;  //Serial Monitor uses for clarity.
int32_t					        wHttpServerCount	= 0;	//Log every nth call at UPLOAD_FILE_WRITE
String					        szLogLine;
BeckFirebase*			      pBeckFBase;
bool                    bWiFiOn           = false;
bool                    bFirebaseOn       = false;
ESP8266WebServer		    oHttpServer(80);
ESP8266HTTPUpdateServer	oHttpUpdateServer(true);

BeckFirebase::BeckFirebase(String sDatabaseURL,String sFirebaseSecret,
		                   String sLogPath, String sMyName){
	sDatabaseURL_		= sDatabaseURL;
	sFirebaseSecret_	= sFirebaseSecret;
	sLogPath_			= sLogPath;
	sMyName_			= sMyName;
	sPushPath_			= sLogPath_ + sMyName_;

	BLogS("BeckFirebase() cstor: Call new Firebase(" + sDatabaseURL_ + ")");
	pFBase_= new Firebase(sDatabaseURL_);

	BLogS("BeckFirebase() cstor: Call Firebase.auth(" + sFirebaseSecret_ + ")");
	pFBase_->auth(sFirebaseSecret_);

	BLogS("BeckFirebase() cstor: Firebase client created to " + sDatabaseURL_);
	return;
}	//BeckFirebase cstor


void BeckFirebase::LogToSerial(String sLogline){
	Serial << sLogline << endl;
	return;
}	//LogToSerial


bool BeckFirebase::LogToFirebase(String sLogline){
	if (bFirebaseOk_) {
		String sJSONPushString= sMakeJSONObject("Log", sLogline);
		FirebasePush push = pFBase_->push(sPushPath_, sJSONPushString);
		if (push.error()) {
			bFirebaseOk_= false;
			BLogS("LogToFirebase(): Firebase push failed, Error: " + String(push.error().message()));
		}	//if(push.error())
	}	//if(bFirebaseOk_)
	else
	{
		//BLogS("LogToFirebase(): bFirebaseOk_ is false, skipped write to Firebase");
	}	//if(bFirebaseOk_)else
	return bFirebaseOk_;
}	//LogToFirebase


void BeckFirebase::LogToBoth(String sLogline){
  //Put a "*" in front of log line if not logging to Firebase
  if (!bFirebaseOk_) {
    sLogline= "*" + sLogline;
  } //if(bFirebaseOk_)else

  String sFullLogline=LOG0 + sLogline;
	LogToSerial  (sFullLogline);
#ifndef NO_FIREBASE
	LogToFirebase(sFullLogline);
#endif
	return;
}	//LogToBoth


String BeckFirebase::GetDatabaseURL(void){
	return sDatabaseURL_;
}	//GetDatabaseURL


String BeckFirebase::GetLogPath(void){
	return sLogPath_;
}	//GetLogPath


String BeckFirebase::GetPushPath(void){
	return sPushPath_;
}	//GetPushPath


String BeckFirebase::sMakeJSONObject(String sName, String sValue){
  String szJSONObject= "{\"";
  szJSONObject += sName;
  szJSONObject += "\": \"";
  szJSONObject += sValue;
  szJSONObject += "\"}";
  return szJSONObject;
} //sMakeJSONObject
//***End of BeckFirebase class methods***


BeckFirebase* StartBeckFirebase(String sDatabaseURL, String sFirebaseSecret, String sLogPath, String sMyName){
	BLogS("BeckFirebase() cstor: Call new Firebase(" + sDatabaseURL + ")");
	BeckFirebase* pBeckFirebase= new BeckFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, sMyName);
	pBeckFBase= pBeckFirebase;
	if (bWiFiOn) {
	  TestFirebase();
	} //if(bWiFiOn)
	else {
    pBeckFBase->bFirebaseOk_= false;
  } //if(bWiFiOn)else
	return(pBeckFBase);
}	//StartBeckFirebase


bool TestFirebase(void){
	BLogS("TestFirebase(): Make test call to Firebase.");
	pBeckFBase->bFirebaseOk_= true;
	if (!BLogF("BeckFirebase() Test")) {
		BLogS("BeckFirebase(): Test to Firebase failed, setting bFirebaseOk_ to false");
		pBeckFBase->bFirebaseOk_= false;
	}	//if(!BLogF("BeckFirebase()Test"))

	return pBeckFBase->bFirebaseOk_;
}	//TestFirebase


void SendInfoToLog(void){
	BLog("SendInfoToLog(): GetDatabaseURL()= |" + pBeckFBase->GetDatabaseURL() + "|");
	BLog("SendInfoToLog(): GetLogPath()= |" + pBeckFBase->GetLogPath() + "|");
	BLog("SendInfoToLog(): GetPushPath()= |" + pBeckFBase->GetPushPath() + "|");

	LogESPValues();
	return;
} //SendInfoToLog


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


void Log(String sLogline){
	BLogS(sLogline);
	return;
}	//Log


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
