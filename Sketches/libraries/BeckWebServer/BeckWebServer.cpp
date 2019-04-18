//BeckWebServer.cpp, 4/18/19a
#include <BeckWebServer.h>
#include <BeckMiniLib.h>
#include <WiFiClient.h>

#ifdef ESP8266
  //#include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
  ESP8266WebServer    oWebServer(80);    //Use normal port 80
#else   //ESP32
  //#include <ESPmDNS.h>
  #include <WebServer.h>
  #include <WiFi.h>
  WebServer           oWebServer(80);
#endif    //ESP8266


void StartWebServer(const char *acHostname){
  Serial << LOG0 << "StartWebServer(): Begin" << endl;
#if 0
  //Use MDNS for host name resolution
  Serial << LOG0 << "StartWebServer(): Start mDNS for " << acHostname << endl;
  if (MDNS.begin(acHostname)) {
    MDNS.addService("http", "tcp", 80);
    Serial << LOG0 << "StartWebServer(): Open http://" << acHostname << "/login to perform an OTA update" << endl;
  } //if(MDNS.begin(acHostname))
  else {
    Serial << LOG0 << "StartWebServer(): Error setting up MDNS responder" << endl;
  } //if(MDNS.begin(acHostname))else
#endif
  oWebServer.begin();
  return;
} //StartWebServer


void HandleWebServer(void){
  oWebServer.handleClient();
  delay(1);
  return;
} //HandleWebServer
//Last line.
