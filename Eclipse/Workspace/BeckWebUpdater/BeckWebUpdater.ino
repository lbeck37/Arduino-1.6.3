static const char szSketchName[]  = "BeckWebUpdater.ino";
static const char szFileDate[]    = "Jan 5, 2016A";
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

const char* host = "esp37F";
const char* ssid = "Aspot24";
const char* password = "Qazqaz11";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void){

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting BeckOTA2.bin 1/5/16F");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
}

void loop(void){
  httpServer.handleClient();
  delay(1);
}


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


