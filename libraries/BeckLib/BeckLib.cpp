//BeckLib.cpp, May 7, 2016
#include <BeckLib.h>
//#define DEBUG_LOGGING
//#define NO_FIREBASE

//Global variables
long					lLineCount= 0;      //Serial Monitor uses for clarity.
String					szLogLine;
BeckFirebase*			pBeckFBase;
ESP8266WebServer		oHttpServer(80);
ESP8266HTTPUpdateServer	oHttpUpdateServer(true);

BeckFirebase::BeckFirebase(String sDatabaseURL,String sFirebaseSecret,
		                   String sLogPath, String sMyName){
	sDatabaseURL_		= sDatabaseURL;
	strFirebaseSecret_	= sFirebaseSecret;
	sLogPath_			= sLogPath;
	sMyName_			= sMyName;
	sPushPath_			= sLogPath_ + sMyName_;

	pFBase_= new Firebase(sDatabaseURL);

	Serial << LOG0 << " BeckFirebase() cstor: Call Firebase.auth('" << strFirebaseSecret_ << "')" << endl;
	pFBase_->auth(strFirebaseSecret_);

	LogToBoth("BeckFirebase() cstor: Firebase client created to " + sDatabaseURL_);
	return;
}	//BeckFirebase cstor


void BeckFirebase::LogToSerial(String sLogline){
	Serial << sLogline << endl;
	return;
}	//LogToSerial


void BeckFirebase::LogToFirebase(String sLogline){
	String sJSONPushString= sMakeJSONObject("Log", sLogline);

	FirebasePush push = pFBase_->push(sPushPath_, sJSONPushString);

	if (push.error()) {
		Serial << LOG0 << " LogToFirebase(): Firebase push failed, Error: " << push.error().message() << endl;
	}	//if(push.error())
	else {
	}	//if(push.error())else
	return;
}	//LogToFirebase


void BeckFirebase::LogToBoth(String sLogline){
	String sFullLogline=LOG0 + " " + sLogline;

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
	BeckFirebase* pBeckFirebase= new BeckFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, sMyName);
	pBeckFBase= pBeckFirebase;
return(pBeckFBase);
}	//StartBeckFirebase


void SendInfoToLog(void){
	pBeckFBase->LogToBoth("SendInfoToLog(): GetDatabaseURL()= |" + pBeckFBase->GetDatabaseURL() + "|");
	pBeckFBase->LogToBoth("SendInfoToLog(): GetLogPath()= |" + pBeckFBase->GetLogPath() + "|");
	pBeckFBase->LogToBoth("SendInfoToLog(): GetPushPath()= |" + pBeckFBase->GetPushPath() + "|");
	  return;
} //SendInfoToLog


void SetupWiFi(const char* pcRouterName, const char* pcRouterPW){
	LogJustToSerial("SetupWiFi(): Setting WiFi mode to WIFI_AP_STA");
	WiFi.mode(WIFI_AP_STA);

	LogJustToSerial("SetupWiFi(): Call WiFi.begin(" + String(pcRouterName) + ", " + String(pcRouterPW) + ")");
	WiFi.begin(pcRouterName, pcRouterPW);

	LogJustToSerial("SetupWiFi(): Call WiFi.waitForConnectResult()");
	while(WiFi.waitForConnectResult() != WL_CONNECTED){
		LogJustToSerial("WiFi failed, retrying.");
		LogJustToSerial("SetupWiFi(): Call WiFi.begin(" + String(pcRouterName) + ", " + String(pcRouterPW) + ")");
		WiFi.begin(pcRouterName, pcRouterPW);
	 }

	LogJustToSerial("SetupWiFi(): WifFi Connected, WiFi.status() returned WL_CONNECTED");

	LogJustToSerial("SetupWiFi(): My WiFi IP address= " + szIPaddress(WiFi.localIP()));
} //SetupWiFi


void SetupHttpServer(const char* acHostname,
					ESP8266WebServer& oHttpServer,
					ESP8266HTTPUpdateServer& oHttpUpdateServer){
  pBeckFBase->LogToBoth("SetupHttpServer(): Call MDNS.begin(" + String(acHostname) + ")");
  MDNS.begin(acHostname);

  pBeckFBase->LogToBoth("SetupHttpServer(): Call oHttpUpdateServer.setup(&oHttpServer)");
  oHttpUpdateServer.setup(&oHttpServer);

  pBeckFBase->LogToBoth("SetupHttpServer(): Call oHttpServer.begin())");
  oHttpServer.begin();

  pBeckFBase->LogToBoth("SetupHttpServer(): Call MDNS.addService(http, tcp, 80)");
  MDNS.addService("http", "tcp", 80);

  pBeckFBase->LogToBoth("SetupHttpServer(): HTTPUpdateServer ready!");
  pBeckFBase->LogToBoth("SetupHttpServer(): Open http://" + String(acHostname) + ".local/update to do OTA Update");
}	//SetupHttpServer


void HandleHttpServer(ESP8266WebServer& oHttpServer){
  oHttpServer.handleClient();
  delay(1);
} //HandleHttpServer


void Log(String sLogline){
	pBeckFBase->LogToBoth(sLogline);
	return;
}	//Log


void LogJustToSerial(String sLogline){
	String sFullLogline=LOG0 + " " + sLogline;
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
