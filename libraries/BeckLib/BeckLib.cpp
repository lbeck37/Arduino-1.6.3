//BeckLib.cpp, May 5, 2016
#include <BeckLib.h>
//#define DEBUG_LOGGING

//Global variables
long		lLineCount= 0;      //Serial Monitor uses for clarity.
String		szLogLine;

BeckFirebase::BeckFirebase(String strDatabaseURL,String strFirebaseSecret) {
	strDatabaseURL_= strDatabaseURL;
	strFirebaseSecret_= strFirebaseSecret;

	Setup();
}


BeckFirebase::BeckFirebase(){
	return;
}


void BeckFirebase::LogToFirebase(String sLogline){
	Serial << LOG0 << " BeckFirebase::LogToFirebase: '" << sLogline << "'" << endl;
	return;
}	//LogToFirebase


int BeckFirebase::Setup(void){
	//oFBase_= Firebase(strDatabaseURL_);
	return 1;
}	//Setup


Firebase SetupFirebase(String acDatabaseURL, String acFirebaseSecret, String& acPushPath, String acMyName){
  //Create Firebase client.
  Serial << LOG0 << " SetupFirebase(): Create Firebase client" << endl;

  Serial << LOG0 << " SetupFirebase(): Call Firebase('" << acDatabaseURL << "')" << endl;
  Firebase oFBase = Firebase(acDatabaseURL);

  Serial << LOG0 << " SetupFirebase(): Call Firebase.auth('" << acFirebaseSecret << "')" << endl;
  oFBase.auth(acFirebaseSecret);

  //Create path to write to by appending my name to the Log prefix.
  acPushPath= acPushPath + acMyName;

  szLogLine=  LOG0 + " SetupFirebase(): Firebase client created to " + acDatabaseURL;
  LogToBoth(oFBase, acPushPath, szLogLine);
  return(oFBase);
}	//SetupFirebase


void SetupWiFi(char* pcRouterName, char* pcRouterPW){
	Serial << LOG0 << " SetupWiFi(): Setting WiFi mode to WIFI_AP_STA" << endl;
	WiFi.mode(WIFI_AP_STA);

	Serial << LOG0 << " SetupWiFi(): Call WiFi.begin(" << pcRouterName << ", " << pcRouterPW << ")" << endl;
	WiFi.begin(pcRouterName, pcRouterPW);

	Serial << LOG0 << " SetupWiFi(): Call WiFi.waitForConnectResult()" << endl;
	while(WiFi.waitForConnectResult() != WL_CONNECTED){
	Serial << LOG0 << " WiFi failed, retrying." << endl;
	Serial << LOG0 << " SetupWiFi(): Call WiFi.begin(" << pcRouterName << ", " << pcRouterPW << ")" << endl;
	  WiFi.begin(pcRouterName, pcRouterPW);
	 }

	szLogLine=  LOG0 + " SetupWiFi(): WifFi Connected, WiFi.status() returned WL_CONNECTED";
	LogToSerial(szLogLine);

	szLogLine=  LOG0 + " SetupWiFi(): My WiFi IP address= " + szIPaddress(WiFi.localIP());
	LogToSerial(szLogLine);
} //SetupWiFi


void SetupHttpServer(char* acHostname,
					ESP8266WebServer& oHttpServer,
					ESP8266HTTPUpdateServer& oHttpUpdateServer){
  Serial << LOG0 << " SetupHttpServer(): Call MDNS.begin(" << acHostname << ")" << endl;
  MDNS.begin(acHostname);

  Serial << LOG0 << " SetupHttpServer(): Call oHttpUpdateServer.setup(&oHttpServer)" << endl;
  oHttpUpdateServer.setup(&oHttpServer);

  Serial << LOG0 << " SetupHttpServer(): Call oHttpServer.begin())" << endl;
  oHttpServer.begin();

  Serial << LOG0 << " SetupHttpServer(): Call MDNS.addService(http, tcp, 80)" << endl;
  MDNS.addService("http", "tcp", 80);

  Serial << LOG0 << " SetupHttpServer(): HTTPUpdateServer ready!" << endl;
  Serial << LOG0 << " SetupHttpServer(): Open http://" << acHostname << ".local/update to do OTA Update" << endl;
}	//SetupHttpServer


void HandleHttpServer(ESP8266WebServer& oHttpServer){
  oHttpServer.handleClient();
  delay(1);
} //HandleHttpServer


String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szTermString += szTime;
  szHeader += szGetTime(millis());
  //szHeader += " ";
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


void LogToSerial(String szLogString){
  Serial << szLogString << endl;
  return;
} //LogToSerial


//LogToBoth() and BlynkLogLine()need multiple versions
//depending on there being a 2nd variable and its type.
void LogToBoth(Firebase& oFBase, String acPushPath, String szLogString){
  LogToSerial(szLogString);
  FbaseLogLine(oFBase, acPushPath, szLogString);
  return;
} //LogToBoth


void FbaseLogLine(Firebase& oFBase, String acPushPath, String szLogString){
	String szJSONPushString= szMakeJSONObject("Log", szLogString);
#ifdef DEBUG_LOGGING
	Serial << LOG0 << " FbaseLogLine(): Called szMakeJSONObject()" << endl;
	Serial << LOG0 << " FbaseLogLine(): Call oFBase.push(acPushPath, szJSONPushString)" << endl;
	Serial << LOG0 << " FbaseLogLine(): acPushPath      = |" << acPushPath << "|" << endl;
	Serial << LOG0 << " FbaseLogLine(): szJSONPushString= |" << szJSONPushString << "|" << endl;
#endif
	FirebasePush push = oFBase.push(acPushPath, szJSONPushString);
	if (push.error()) {
		Serial << LOG0 << " FbaseLogLine(): Firebase push failed, Error: " << push.error().message() << endl;
	}	//if(push.error())
	else {
#ifdef DEBUG_LOGGING
	Serial << LOG0 << " FbaseLogLine(): Firebase returned not Error: " << endl;
#endif
	}	//if(push.error())else
  return;
} //FbaseLogLine


String szMakeJSONObject(String szName, String szValue){
  String szJSONObject= "{\"";
  szJSONObject += szName;
  szJSONObject += "\": \"";
  szJSONObject += szValue;
  szJSONObject += "\"}";
  return szJSONObject;
} //szMakeJSONObject


String szIPaddress(IPAddress oIP){
  String szDot= ".";
  String szOctetIP= oIP[0] + szDot +  oIP[1] + szDot  + oIP[2] + szDot +  oIP[3];
  return szOctetIP;
} //szIPaddress
//Last line.
