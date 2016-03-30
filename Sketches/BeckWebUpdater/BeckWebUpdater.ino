static const char acSketchName[]  = "BeckWebUpdater.ino";
static const char acFileDate[]    = "Mar 30, 2016C";
// 1/5/16 Get running on V64 eclipseArduino

#include <Streaming.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#define LOG0    	szLogLineHeader(++lLineCount)
static long         lLineCount            = 0;      //Serial Monitor uses for clarity.
static const long   lSerialMonitorBaud    = 115200;
static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

static const char   acRouterName[]        = "Aspot24";
static const char   acRouterPW[]          = "Qazqaz11";
static const char   acHostname[]          = "esp39";

ESP8266WebServer    			oHttpServer(80);
ESP8266HTTPUpdateServer 	oHttpUpdateServer(true);

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << " setup(): Sketch: " << acSketchName << ", " << acFileDate << endl;

  Serial << LOG0 << " setup(): Setting WiFi mode to WIFI_AP_STA" << endl;
  WiFi.mode(WIFI_AP_STA);

  Serial << LOG0 << " setup(): Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
  WiFi.begin(acRouterName, acRouterPW);

  Serial << LOG0 << " setup(): Call WiFi.waitForConnectResult()" << endl;
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
	Serial << LOG0 << " setup(): WiFi failed, retrying." << endl;
	Serial << LOG0 << " setup(): Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
    WiFi.begin(acRouterName, acRouterPW);
   }
  SetupHttpServer();
}	//setup


void loop(void){
  HandleHttpServer();
}	//loop


void SetupHttpServer() {
  MDNS.begin(acHostname);
  oHttpUpdateServer.setup(&oHttpServer);
  oHttpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial << LOG0 << " SetupHttpServer(): HTTPUpdateServer ready! Open http://" << acHostname << ".local/update in your browser" << endl;
}	//SetupHttpServer


void HandleHttpServer() {
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
  szString+= String(szAddZeros(sMsec, 3)) + " ";     //Send with trailing blank to seperate from next field.
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

//Last line.
