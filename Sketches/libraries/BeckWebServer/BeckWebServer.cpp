//BeckWebServer.cpp, 4/17/19a
#include <BeckWebServer.h>
//#include "BeckWebServerHTML.h"
//#include "BeckOTALibHTML.h"
//#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <WiFiClient.h>

#ifdef ESP8266
  //#include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
#else   //ESP32
  //#include <ESPmDNS.h>
  #include <WebServer.h>
  #include <WiFi.h>
#endif    //ESP8266


#ifdef ESP8266
  ESP8266WebServer    oWebServer(80);    //Use normal port 80
#else   //ESP32
  WebServer           oWebServer(80);
#endif    //ESP8266

#ifdef ESP8266
  #ifndef UPDATE_SIZE_UNKNOWN
    #define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
  #endif
#endif    //ESP8266

void StartWebServer(const char *acHostname){
  Serial << LOG0 << "StartWebServer(): Begin" << endl;
/*
  //Use MDNS for host name resolution
  Serial << LOG0 << "StartWebServer(): Start mDNS for " << acHostname << endl;
  if (!MDNS.begin(acHostname)) {
    Serial << LOG0 << "StartWebServer(): Error setting up MDNS responder" << endl;
  } //if (!MDNS.begin(acHostname))
*/
#if 0
  oWebServer.on("/login", HTTP_GET, []() {
    oWebServer.sendHeader("Connection", "close");
    //oWebServer.send(200, "text/html", loginIndex);
    oWebServer.send(200, "text/html", acOTA_LoginHTML);
  });
  oWebServer.on("/serverIndex", HTTP_GET, []() {
    oWebServer.sendHeader("Connection", "close");
    //oWebServer.send(200, "text/html", serverIndex);
    oWebServer.send(200, "text/html", acOTA_ServerHTML);
  });
  oWebServer.on("/update", HTTP_POST, []() {
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = oWebServer.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      //flashing firmware to ESP
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
#endif  //0
/*
  MDNS.addService("http", "tcp", 80);
  Serial << LOG0 << "StartWebServer(): Open http://" << acHostname << "/login to perform an OTA update" << endl;
*/
  oWebServer.begin();
  return;
} //StartWebServer


void HandleWebServer(void){
  oWebServer.handleClient();
  delay(1);
  return;
} //HandleWebServer
//Last line.
