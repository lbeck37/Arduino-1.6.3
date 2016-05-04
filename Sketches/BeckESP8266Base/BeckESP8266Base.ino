String acSketchName  = "BeckESP8266Base.ino";
String acFileDate    = "May 3, 2016_HP7D";

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
*/
static char       acRouterName[]        = "Aspot24";
static char       acRouterPW[]          = "Qazqaz11";

ESP8266WebServer        oHttpServer(80);
ESP8266HTTPUpdateServer   oHttpUpdateServer(true);

static String     		acDatabaseURL   	= "intense-fire-3958.firebaseio.com";
static String     		acFirebaseSecret  	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static char           	acMyName[]          = "esp1101Dev";   //Beck, Dev type sensor, #1
static String         	acPushPath      	= "/Logs/";

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << " Sketch: " << acSketchName << ", " << acFileDate << endl;

  SetupWiFi(acRouterName, acRouterPW);

/*
  //Create Firebase client.
  Serial << LOG0 << " Create Firebase client" << endl;

  acPushPath= acPushPath + acMyName;
  Serial << LOG0 << " Call Firebase('" << acDatabaseURL << "')" << endl;
  Firebase oFBase = Firebase(acDatabaseURL);

  Serial << LOG0 << " Call Firebase.auth('" << acFirebaseSecret << "')" << endl;
  oFBase.auth(acFirebaseSecret);
*/

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
} //setup


void loop(void){
  HandleHttpServer(oHttpServer);
} //loop
//Last line.
