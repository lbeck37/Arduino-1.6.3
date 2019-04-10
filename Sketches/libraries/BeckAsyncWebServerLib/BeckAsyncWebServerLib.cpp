//BeckWebServerLib.cpp, 4/9/19d
#include <BeckAsyncWebServerLib.h>
#include "ESPAsyncWebServer.h"
#include <Streaming.h>

#include "BeckAsyncWebServerHTML.h"

// Create AsyncWebServer object on port 80
AsyncWebServer BiotaAsyncWebServer(80);

// Replaces placeholder with DHT values
//String processor(const String& var){
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


void StartAsyncWebServer(){
  Serial << "BeckWebServerLib.cpp: StartWebServer(): Start" << endl;
  // Route for root / web page
  BiotaAsyncWebServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, CallBackFunc);
  });
  BiotaAsyncWebServer.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readDHTTemperature().c_str());
    request->send_P(200, "text/plain", readDummyTemperature().c_str());
  });
  BiotaAsyncWebServer.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send_P(200, "text/plain", readDHTHumidity().c_str());
    request->send_P(200, "text/plain", readDummyHumidity().c_str());
  });

  // Start server
  BiotaAsyncWebServer.begin();
  return;
} //StartAsyncWebServer
//Last line
