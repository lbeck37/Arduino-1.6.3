static const char acSketchName[]  = "BeckESP8266Base.ino";
static const char acFileDate[]    = "Apr 24, 2016D";
// 1/5/16 Get running on V64 eclipseArduino

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
*/
static const char   		acRouterName[]        = "Aspot24";
static const char   		acRouterPW[]          = "Qazqaz11";
static char   				acHostname[]          = "esp40";

ESP8266WebServer    		oHttpServer(80);
ESP8266HTTPUpdateServer 	oHttpUpdateServer(true);

static const String 		acDatabaseURL		= "intense-fire-3958.firebaseio.com";
static const String 		acFirebaseSecret	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static const String 		acPushPath			= "/logs";
//String	szLogLine;

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << " Sketch: " << acSketchName << ", " << acFileDate << endl;

  Serial << LOG0 << " Setting WiFi mode to WIFI_AP_STA" << endl;
  WiFi.mode(WIFI_AP_STA);

  Serial << LOG0 << " Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
  WiFi.begin(acRouterName, acRouterPW);

  Serial << LOG0 << " Call WiFi.waitForConnectResult()" << endl;
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
	Serial << LOG0 << " WiFi failed, retrying." << endl;
	Serial << LOG0 << " Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
    WiFi.begin(acRouterName, acRouterPW);
   }

  szLogLine=  LOG0 + " *setup(): WifFi Connected, WiFi.status() returned WL_CONNECTED";
  LogToSerial(szLogLine);

  //Create Firebase client.
  Serial << LOG0 << " Create Firebase client" << endl;

  //Creation and call to auth() need to be separate, return value from auth() is not same Firebase object.
  Serial << LOG0 << " Call Firebase('" << acDatabaseURL << "')" << endl;
  Firebase oFBase = Firebase(acDatabaseURL);
  oFBase.PrintPrivates();

  Serial << LOG0 << " Call Firebase.auth('" << acFirebaseSecret << "')" << endl;
  oFBase.auth(acFirebaseSecret);
  oFBase.PrintPrivates();

  //Firebase is up, start logging to both
  szLogLine= LOG0 + " *setup(): Sketch " + acSketchName + ", version: " + acFileDate;
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine=  LOG0 + " *setup(): Firebase client created to " + acDatabaseURL;
  LogToBoth(oFBase, acPushPath, szLogLine);

  SetupHttpServer(acHostname, oHttpServer, oHttpUpdateServer);

  szLogLine= LOG0 + " *setup(): Open http://" + acHostname + ".local/update in browser to do OTA Update";
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine= LOG0 + " *setup() Done in " + acSketchName + ", version: " + acFileDate;
  LogToBoth(oFBase, acPushPath, szLogLine);
  return;
}	//setup


void loop(void){
  HandleHttpServer(oHttpServer);
}	//loop
//Last line.
