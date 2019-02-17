//BeckESP_OTAWebServerLib.cpp, Beck 2/3b/19
#include <BeckESP_OTAWebServerLib.h>
#include "BeckESP_OTAWebServerPages.h"
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <Updater.h>
#else   //ESP32
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include <WebServer.h>
  #include <Update.h>
#endif    //ESP8266

//const char*         acServerIndex         = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
unsigned long       _ulUpdateTimeoutMsec   = 0;
bool                _bOTA_Started         = false;   //Turns off Blynk.

#ifdef ESP8266
  ESP8266WebServer    oWebServer(80);
#else   //ESP32
  WebServer           oWebServer(80);
#endif    //ESP8266

#ifdef ESP8266
  #ifndef UPDATE_SIZE_UNKNOWN
    #define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
  #endif
#endif    //ESP8266

void SetupOTAServer(const char *acHostname) {
  Serial << LOG0 << "SetupOTAServer(): Start mDNS for " << acHostname << endl;
  /*use mdns for host name resolution*/
  if (!MDNS.begin(acHostname)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    } //while
  } //if (!MDNS.begin(acHostname))
  /*return index page which is stored in serverIndex */
  oWebServer.on("/", HTTP_GET, []() {
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/html", loginIndex);
  });
  oWebServer.on("/serverIndex", HTTP_GET, []() {
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
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
      /* flashing firmware to ESP*/
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
  oWebServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial << LOG0 << "SetupOTAServer(): Open http://" << acHostname << ".local to perform an OTA update" << endl;
  return;
} //SetupOTAServer


void HandleOTAServer(void){
  oWebServer.handleClient();
  delay(1);
  return;
} //HandleOTAServer
//Last line.
