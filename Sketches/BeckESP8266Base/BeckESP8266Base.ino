String acSketchName  = "BeckESP8266Base.ino";
String acFileDate    = "May 4, 2016_HP7E";

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
*/
static char       acRouterName[]        = "Aspot24";
static char       acRouterPW[]          = "Qazqaz11";

ESP8266WebServer        oHttpServer(80);
ESP8266HTTPUpdateServer   oHttpUpdateServer(true);

static String     		strDatabaseURL   	= "intense-fire-3958.firebaseio.com";
static String     		strFirebaseSecret  	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static char           	acMyName[]          = "esp1101Dev";   //Beck, Dev type sensor, #1
static String         	acPushPath      	= "/Logs/";

//extern Firebase oFBase;
//BeckFirebase	oBeckFirebase(strDatabaseURL, strFirebaseSecret);


void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << " Sketch: " << acSketchName << ", " << acFileDate << endl;

  SetupWiFi(acRouterName, acRouterPW);

  Firebase oFBase = SetupFirebase(strDatabaseURL, strFirebaseSecret, acPushPath, acMyName);
  //oFBase = SetupFirebase(acDatabaseURL, acFirebaseSecret, acPushPath, acMyName);

  SetupHttpServer(acMyName, oHttpServer, oHttpUpdateServer);

  szLogLine= LOG0 + " *setup(): Open http://" + acMyName + ".local/update in browser to do OTA Update";
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine= LOG0 + " *setup(): Firebase URL= https://" + strDatabaseURL + acPushPath;
  LogToBoth(oFBase, acPushPath, szLogLine);

  szLogLine= LOG0 + " *setup() Done in " + acSketchName + ", version: " + acFileDate;
  LogToBoth(oFBase, acPushPath, szLogLine);
  return;
} //setup


void loop(void){
  HandleHttpServer(oHttpServer);
} //loop
//Last line.
