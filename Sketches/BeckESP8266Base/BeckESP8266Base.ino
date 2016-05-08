String acSketchName  = "BeckESP8266Base.ino";
String acFileDate    = "May 7, 2016_HP7AS";

#include <BeckLib.h>
/*
static const char   acRouterName[]        = "TrailheadBoise";
static const char   acRouterPW[]          = "Trailhead2015";
*/
static const char       		acRouterName[]      = "Aspot24";
static const char       		acRouterPW[]        = "Qazqaz11";
static const String     		sDatabaseURL   		= "intense-fire-3958.firebaseio.com";
static const String     		sFirebaseSecret  	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static const char           	acMyURL[]           = "esp1101Dev";   //Beck, Dev type sensor, #1
//static const char           	acMyFbaseName[]     = "BeckESP8266Base_1dotESP";
//static const char           	acMyFbaseName[]     = "BeckESP8266Base_2dotESP";//Has WiFi problem.
static const char           	acMyFbaseName[]     = "BeckESP8266Base_3dotESP";
static const String         	sLogPath      		= "/Logs/";
ESP8266WebServer        		oHttpServer(80);
ESP8266HTTPUpdateServer   		oHttpUpdateServer(true);

//Function proto
void SendInfoToLog(void);

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl;
  LogJustToSerial("setup(): Initialized serial to " + String(lSerialMonitorBaud) + " baud");
  LogJustToSerial("Sketch: " + acSketchName + ", " + acFileDate);

  SetupWiFi(acRouterName, acRouterPW);

  StartBeckFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, acMyFbaseName);
  SendInfoToLog();

  SetupHttpServer(acMyURL, oHttpServer, oHttpUpdateServer);

  pBeckFBase->LogToBoth("setup(): Done, Sketch: " + acSketchName + ", version: " + acFileDate);
  return;
} //setup


void loop(void){
  HandleHttpServer(oHttpServer);
} //loop


void SendInfoToLog(void){
	  pBeckFBase->LogToBoth("SendInfoToLog(): GetDatabaseURL()= |" + pBeckFBase->GetDatabaseURL() + "|");
	  pBeckFBase->LogToBoth("SendInfoToLog(): GetLogPath()= |" + pBeckFBase->GetLogPath() + "|");
	  pBeckFBase->LogToBoth("SendInfoToLog(): GetPushPath()= |" + pBeckFBase->GetPushPath() + "|");
} //loop
//Last line.
