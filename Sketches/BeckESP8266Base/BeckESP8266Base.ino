String acSketchName  = "BeckESP8266Base.ino";
String acFileDate    = "May 7, 2016_HP7AJ";

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
static const char           	acMyFbaseName[]     = "BeckESP8266Base_1dotESP";
static const String         	sLogPath      		= "/Logs/";
static const String         	sPushPath      		= "/Logs/BeckESP8266Base_1dotESP";
ESP8266WebServer        		oHttpServer(80);
ESP8266HTTPUpdateServer   		oHttpUpdateServer(true);
//Firebase*						pFBaseOriginal;

void setup(void){
  Serial.begin(lSerialMonitorBaud);
  Serial << endl;
  LogJustToSerial("setup(): Initialized serial to " + String(lSerialMonitorBaud) + " baud");
  LogJustToSerial("Sketch: " + acSketchName + ", " + acFileDate);

  SetupWiFi(acRouterName, acRouterPW);

/*
  Firebase oFBase = SetupFirebase(sDatabaseURL, sFirebaseSecret);
  pFBaseOriginal= &oFBase;
  LogJustToSerial("setup(): After SetupFirebase(), sLogPath= " + sLogPath);
*/

  StartBeckFirebase(sDatabaseURL, sFirebaseSecret, sLogPath, acMyFbaseName);

  pBeckFBase->LogToBoth("setup(): GetDatabaseURL()= |" + pBeckFBase->GetDatabaseURL() + "|");
  pBeckFBase->LogToBoth("setup(): GetLogPath()= |" + pBeckFBase->GetLogPath() + "|");
  pBeckFBase->LogToBoth("setup(): GetPushPath()= |" + pBeckFBase->GetPushPath() + "|");
  pBeckFBase->LogToBoth("setup() Done in " + acSketchName + ", version: " + acFileDate);

  SetupHttpServer(acMyURL, oHttpServer, oHttpUpdateServer);

  pBeckFBase->LogToBoth("setup(): Back from SetupHttpServer()");

  String sTestLog= LOG0 + "setup(): Test " + acFileDate;
  LogJustToSerial(sTestLog);
  //pBeckFBase->LogToFirebase(sTestLog);		//This is only one going to Firebase.

  pBeckFBase->LogToBoth("setup(): Back from LogToFirebase()");

/*
  szLogLine=  LOG0 + " setup(): Sketch: " + acSketchName + ", version: " + acFileDate;
  LogToBoth(oFBase, sPushPath, szLogLine);
*/
  pBeckFBase->LogToBoth("setup(): Done, Sketch: " + acSketchName + ", version: " + acFileDate);
  return;
} //setup


void loop(void){
  HandleHttpServer(oHttpServer);
} //loop
//Last line.
