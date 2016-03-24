static const char acSketchName[]  = "BeckWebUpdater.ino";
static const char acFileDate[]    = "Mar 24, 2016C";
//static const char acFileDate[]    = "Feb 9, 2016B";
// 1/5/16 Get running on V64 eclipseArduino
// To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update

//#include <Streaming.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

//#define LOG0    szLogLineHeader(++lLineCount)
//static long         lLineCount            = 0;      //Serial Monitor uses for clarity.

static const char   acRouterName[]        = "Aspot24";
static const char   acRouterPW[]          = "Qazqaz11";
static const char   acHostname[]          = "esp38";

ESP8266WebServer    			oHttpServer(80);
ESP8266HTTPUpdateServer 	oHttpUpdateServer(true);

void setup(void){

  Serial.begin(115200);
  Serial.println();
  Serial.printf("setup(): Booting %s %s\n", acSketchName, acFileDate);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(acRouterName, acRouterPW);

  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(acRouterName, acRouterPW);
    Serial.println("WiFi failed, retrying.");
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
  Serial.printf("SetupHttpServer(): HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", acHostname);
}	//SetupHttpServer


void HandleHttpServer() {
  oHttpServer.handleClient();
  delay(1);
} //HandleHttpServer


/*String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szTermString += szTime;
  szHeader += szGetTime(millis());
  //szHeader += " ";
  return szHeader;
} //szLogLineHeader
*/
//Last line.
