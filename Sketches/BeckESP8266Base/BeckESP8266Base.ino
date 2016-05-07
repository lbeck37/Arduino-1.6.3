String acSketchName  = "BeckESP8266Base.ino";
String acFileDate    = "May 6, 2016_HP7U";

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
*/
static char       acRouterName[]        = "Aspot24";
static char       acRouterPW[]          = "Qazqaz11";

ESP8266WebServer        oHttpServer(80);
ESP8266HTTPUpdateServer   oHttpUpdateServer(true);

static String     		sDatabaseURL   		= "intense-fire-3958.firebaseio.com";
static String     		sFirebaseSecret  	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static char           	acMyURL[]           = "esp1101Dev";   //Beck, Dev type sensor, #1
static char           	acMyFbaseName[]     = "BeckESP8266Base_1dotESP";
static String         	sLogPath      		= "/Logs/";

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl;
  LogJustToSerial("setup(): Initialized serial to " + String(lSerialMonitorBaud) + " baud");
  LogJustToSerial("Sketch: " + acSketchName + ", " + acFileDate);

  SetupWiFi(acRouterName, acRouterPW);

  Firebase oFBase = SetupFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, acMyFbaseName);

  StartBeckFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, acMyFbaseName);

  SetupHttpServer(acMyURL, oHttpServer, oHttpUpdateServer);

  pBeckFBase->LogToBoth("setup(): BeckFirebase.GetDatabaseURL()= |" + pBeckFBase->GetDatabaseURL() + "|");
  pBeckFBase->LogToBoth("setup(): BeckFirebase.GetLogPath()= |" + pBeckFBase->GetLogPath() + "|");
  pBeckFBase->LogToBoth("setup() Done in " + acSketchName + ", version: " + acFileDate);
  return;
} //setup


void loop(void){
  HandleHttpServer(oHttpServer);
} //loop
//Last line.
