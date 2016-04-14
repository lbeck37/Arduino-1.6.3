static const char acSketchName[]  = "BeckWebUpdater.ino";
static const char acFileDate[]    = "Apr 14, 2016C";
// 1/5/16 Get running on V64 eclipseArduino

#include <BeckLib.h>

static const char   acRouterName[]        = "Aspot24";
static const char   acRouterPW[]          = "Qazqaz11";
//static const char   acRouterName[]        = "TrailheadBoise";
//static const char   acRouterPW[]          = "Trailhead2015";
static char   		acHostname[]          = "esp39";

ESP8266WebServer    		oHttpServer(80);
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
  SetupHttpServer(acHostname, oHttpServer, oHttpUpdateServer);
}	//setup


void loop(void){
  HandleHttpServer(oHttpServer);
}	//loop
//Last line.