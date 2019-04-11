//BeckAsyncWebServerLib.cpp, 4/10/19b
#include <BeckAsyncWebServerLib.h>
#include <BeckMiniLib.h>
#include "ESPAsyncWebServer.h"
#include <Streaming.h>
#ifdef ESP8266
  #include <ESP8266mDNS.h>
#else   //ESP32
  #include <ESPmDNS.h>
#endif    //ESP8266

#include "BeckAsyncWebServerHTML.h"
AsyncWebServer oAsyncWebServer(80);

String CallBackFunc(const String& var){
  if(var == "TEMPERATURE"){
    //return readDHTTemperature();
    return "99.99";
  }
  else if(var == "HUMIDITY"){
    //return readDHTHumidity();
    return "99.99";
  }
  return String();
} //CallBackFunc


String readDummyTemperature() {
  return "68.37";
} //readDummyTemperature


String readDummyHumidity() {
  return "70.00";
} //readDummyHumidity


void StartAsyncWebServer(const char *acHostname){
  Serial << LOG0 << "StartAsyncWebServer(): Begin" << endl;
  //Use MDNS for host name resolution
  Serial << LOG0 << "StartAsyncWebServer(): Start mDNS for " << acHostname << endl;
  if (!MDNS.begin(acHostname)) {
    Serial << LOG0 << "StartAsyncWebServer(): Error setting up MDNS responder" << endl;
    while (1) {
      delay(1000);
    } //while
  } //if (!MDNS.begin(acHostname))

  Serial << LOG0 << "StartAsyncWebServer(): Set up handlers" << endl;
  oAsyncWebServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, CallBackFunc);
  });
  oAsyncWebServer.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readDHTTemperature().c_str());
    request->send_P(200, "text/plain", readDummyTemperature().c_str());
  });
  oAsyncWebServer.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readDHTHumidity().c_str());
    request->send_P(200, "text/plain", readDummyHumidity().c_str());
  });

  // Start server
  Serial << LOG0 << "StartAsyncWebServer(): Call oAsyncWebServer.begin()" << endl;
  oAsyncWebServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial << LOG0 << "StartAsyncWebServer(): Open http://" << acHostname << " to access webpage" << endl;
  Serial << LOG0 << "StartAsyncWebServer(): Open http://" << WiFi.localIP() << " to access webpage" << endl;
  return;
} //StartAsyncWebServer
//Last line
