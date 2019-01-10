const char szSketchName[]  = "BeckE8266_GuideWebServerPOST.ino";
const char szFileDate[]    = "Lenny 1/9/19aa";

#include <BeckE8266WiFiLib.h>
#include <Streaming.h>

static const int      _wLedPin              = 2;
char                  _szDNSName[]          = "beckdev1";

void setup(void){
  Serial.begin(115200);
  delay(10);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  pinMode(_wLedPin, OUTPUT);
  _oStationIPAddress= SetupWiFi();
  _oAccessPtIPAddress= SetupAccessPoint();
  SetupWebServer(_oAccessPtIPAddress);
  SetupmDNS(_oStationIPAddress, _szDNSName);
  return;
} //setup


void loop(void){
  pConfigWiFiServer->handleClient();    //Listen for HTTP requests from clients
  return;
} //loop

//Last line.
