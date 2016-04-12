//BeckLib.cpp
#include <BeckLib.h>

void SetupHttpServer(char* acHostname, ESP8266WebServer oHttpServer, ESP8266HTTPUpdateServer oHttpUpdateServer){
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


void HandleHttpServer(ESP8266WebServer oHttpServer){
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


void FbaseLogLine(Firebase oFBase, String acPushPath, String szLogString){
	String szPushString= szMakeJSONObject("Log", szLogString);
	//Serial << LOG0 << " setup(): Call oFBase.push(" << acPushPath << ", " << szPushString << ")" << endl;
	FirebasePush push = oFBase.push(acPushPath, szPushString);
	if (push.error()) {
		Serial << LOG0 << " FbaseLogLine(): Firebase push failed, Error: " << push.error().message() << endl;
		return;
	}	//if(push.error())
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


//LogToBoth() and BlynkLogLine()have multiple versions
//depending on there being a 2nd variable and its type.
void LogToBoth(Firebase oFBase, String acPushPath, String szLogString){
  Serial << szLogString << endl;
  FbaseLogLine(oFBase, acPushPath, szLogString);
  return;
} //LogToBoth
//Last line.
