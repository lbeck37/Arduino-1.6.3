static const char acSketchName[]  = "BeckESP8266Base.ino";
static const char acFileDate[]    = "Apr 25, 2016H1";
// 1/5/16 Get running on V64 eclipseArduino

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
*/
static const char   		acRouterName[]        = "Aspot24";
static const char   		acRouterPW[]          = "Qazqaz11";

ESP8266WebServer    		oHttpServer(80);
ESP8266HTTPUpdateServer 	oHttpUpdateServer(true);

static const String 		acDatabaseURL		= "intense-fire-3958.firebaseio.com";
static const String 		acFirebaseSecret	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static char   				acMyName[]          = "esp1101Dev";		//Beck, Dev sensor, #1
//static const String 		acPushPath			= "/logs";
//static String 				acPushPath			= "/Logs/" + acMyName;
static String 				acPushPath			= "/Logs/";

//static char   			acMyName[]          = "esp40";

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

  szLogLine=  LOG0 + " setup(): WifFi Connected, WiFi.status() returned WL_CONNECTED";
  LogToSerial(szLogLine);

  szLogLine=  LOG0 + " setup(): My WiFi IP address= " + szIPaddress(WiFi.localIP());
  LogToSerial(szLogLine);

  //Create Firebase client.
  Serial << LOG0 << " Create Firebase client" << endl;

  acPushPath= acPushPath + acMyName;
  Serial << LOG0 << " Call Firebase('" << acDatabaseURL << "')" << endl;
  Firebase oFBase = Firebase(acDatabaseURL);

  Serial << LOG0 << " Call Firebase.auth('" << acFirebaseSecret << "')" << endl;
  oFBase.auth(acFirebaseSecret);

  //Firebase is up, start logging to both
  szLogLine= LOG0 + " *setup(): Sketch " + acSketchName + ", version: " + acFileDate;
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine=  LOG0 + " *setup(): Firebase client created to " + acDatabaseURL;
  LogToBoth(oFBase, acPushPath, szLogLine);

  SetupHttpServer(acMyName, oHttpServer, oHttpUpdateServer);

  szLogLine= LOG0 + " *setup(): Open http://" + acMyName + ".local/update in browser to do OTA Update";
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine= LOG0 + " *setup(): Firebase URL= https://" + acDatabaseURL + acPushPath;
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine= LOG0 + " *setup() Done in " + acSketchName + ", version: " + acFileDate;
  LogToBoth(oFBase, acPushPath, szLogLine);
  return;
}	//setup


void loop(void){
  HandleHttpServer(oHttpServer);
}	//loop
//Last line.
