//BeckOTALib.cpp, 4/17/19a
#if 1
#include <BeckOTALib.h>
#include "BeckOTALibHTML.h"
//#include <BeckAsyncWebServerLib.h>
//#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <BeckWebServer.h>
//#include <ArduinoOTA.h>
//#include <WiFiClient.h>
/*
#ifdef ESP8266
  #include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <Updater.h>
#else   //ESP32   //Not tested
  #include <ESPmDNS.h>
  #include <WebServer.h>
  #include <Update.h>
#endif    //ESP8266
*/

//const char*         acServerIndex         = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
/*
unsigned long       _ulUpdateTimeoutMsec   = 0;
bool                _bOTA_Started         = false;   //Turns off Blynk.

#ifdef ESP8266
  ESP8266WebServer    oWebServer(81);    //OTA uses port 81, ie: 192.168.0.169:81/login
#else   //ESP32
  WebServer           oWebServer(81);
#endif    //ESP8266
*/

#ifdef ESP8266
  #ifndef UPDATE_SIZE_UNKNOWN
    #define UPDATE_SIZE_UNKNOWN 0xFFFFFFFF
  #endif
#endif    //ESP8266

void SetupOTAWebPages(){
  Serial << LOG0 << "SetupOTAWebPages(): Begin" << endl;

  oWebServer.on("/login", HTTP_GET, []() {
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/html", acOTA_LoginHTML);
  });
  oWebServer.on("/serverIndex", HTTP_GET, []() {
    oWebServer.sendHeader("Connection", "close");
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
  return;
} //SetupOTAWebPages

/*
void HandleOTAServer(void){
  oWebServer.handleClient();
  delay(1);
  return;
} //HandleOTAServer
*/
#endif  //0 or 1
//Last line.
