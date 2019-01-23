//BeckE32_OTAWebServerLib.cpp, Beck 1/21/19
#include <BeckMiniLib.h>
#include <BeckE32_OTAWebServerLib.h>
#include "BeckE32_OTAWebServerPages.h"
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

//const char*         acServerIndex         = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
unsigned long       _ulUpdateTimeoutMsec   = 0;
bool                _bOTA_Started         = false;   //Turns off Blynk.

//ESP8266WebServer    oWebServer(80);
WebServer oWebServer(80);

//Function prototypes
void HandleOTAUpdate    (void);
void HandleOTAFileEnd   (HTTPUpload& stHTTPUploadLocal);
void PauseBlynk         (void);

void SetupOTAServer(const char *acHostname) {
  Serial << LOG0 << "SetupOTAServer(): Start mDNS for " << acHostname << endl;
  /*use mdns for host name resolution*/
  if (!MDNS.begin(acHostname)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    } //while
  } //if (!MDNS.begin(acHostname))
  //Serial << LOG0 << "SetupOTAServer(): mDNS responder started" << endl;

  Serial << LOG0 << "SetupOTAServer(): Set up callbacks" << endl;
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
  Serial << LOG0 << "SetupOTAServer(): Call oWebServer.begin()" << endl;
  oWebServer.begin();
  Serial << LOG0 << "SetupOTAServer(): Back from oWebServer.begin()" << endl;

  //MDNS.addService("http", "tcp", 80);
  //Serial << LOG0 << "SetupOTAServer(): Open http://" << szOTAServerHostName << ".local to perform an OTA update" << endl;
  Serial << LOG0 << "SetupOTAServer(): Open http://" << acHostname << ".local to perform an OTA update" << endl;
  //Serial << "SetupOTAServer(): Access this device using " << WiFi.localIP() << " or " << acHostname << ".local" << endl;
  return;
} //SetupOTAServer


void HandleOTAServer(void){
  oWebServer.handleClient();
  delay(1);
  return;
} //HandleOTAServer

#if 0
void HandleOTAUpdate() {
  //upload() returns oHttpServer._currentUpload which is an HTTPUpload struct
  HTTPUpload& stHTTPUpload = oWebServer.upload();
  if (stHTTPUpload.status == UPLOAD_FILE_START) {
    PauseBlynk();
    //Serial.setDebugOutput(true);
    Serial.setDebugOutput(false);   //Beck 1/5/19
    WiFiUDP::stopAll();
    uint32_t ulMaxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    Serial << LOG0 << " HandleUpdate(): Update status     = UPLOAD_FILE_START" << endl;
    Serial << LOG0 << " HandleUpdate(): ulMaxSketchSpace  = " << ulMaxSketchSpace << endl;
    Serial << LOG0 << " HandleUpdate(): Update filename   = " << stHTTPUpload.filename << endl;
    Serial << LOG0 << " HandleUpdate(): Update name       = " << stHTTPUpload.name << endl;
    Serial << LOG0 << " HandleUpdate(): Update type       = " << stHTTPUpload.type << endl;
    Serial << LOG0 << " HandleUpdate(): Update totalSize  = " << stHTTPUpload.totalSize << endl;
    Serial << LOG0 << " HandleUpdate(): Update currentSize= " << stHTTPUpload.currentSize << endl;
    if (!Update.begin(ulMaxSketchSpace)) { //start with max available size
      Update.printError(Serial);
    } //if(!Update.begin(maxSketchSpace))
  } //if(WiFi.waitForConnectResult()==WL_CONNECTED)
  else if (stHTTPUpload.status == UPLOAD_FILE_WRITE) {
    //Serial << LOG0 << " Handle /update HTTP_POST: UPLOAD_FILE_WRITE, upload.currentSize= " << stHTTPUpload.currentSize << endl;
    if (Update.write(stHTTPUpload.buf, stHTTPUpload.currentSize)
        != stHTTPUpload.currentSize) {
      Update.printError(Serial);
    } //if(Update.write(upload.buf, upload.currentSize) != upload.currentSize)
  } //else if(upload.status==UPLOAD_FILE_WRITE)
  else if (stHTTPUpload.status == UPLOAD_FILE_END) {
    Serial << LOG0 << " HandleUpdate(): Update status     = UPLOAD_FILE_END" << endl;
    Serial << LOG0 << " HandleUpdate(): Update filename   = " << stHTTPUpload.filename << endl;
    Serial << LOG0 << " HandleUpdate(): Update name       = " << stHTTPUpload.name << endl;
    Serial << LOG0 << " HandleUpdate(): Update type       = " << stHTTPUpload.type << endl;
    Serial << LOG0 << " HandleUpdate(): Update totalSize  = " << stHTTPUpload.totalSize << endl;
    Serial << LOG0 << " HandleUpdate(): Update currentSize= " << stHTTPUpload.currentSize << endl;
    HandleOTAFileEnd(stHTTPUpload);
#if false
    if (Update.end(true)) { //true to set the size to the current progress
      //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      Serial << LOG0
          << " HandleUpdate(): UPLOAD_FILE_END (rebooting?), upload.totalSize= "
          << stHTTPUpload.totalSize << endl;
    } //if(Update.end(true))
    else {
      Update.printError(Serial);
    } //if(Update.end(true))else
    Serial.setDebugOutput(false);
#endif  //false
  } //else if(upload.status==UPLOAD_FILE_END)
  yield();
  return;
} //HandleOTAUpdate


void HandleOTAFileEnd(HTTPUpload& stHTTPUploadLocal) {
  if (Update.end(true)) { //true to set the size to the current progress
    Serial << LOG0 << " HandleOTAFileEnd(): UPLOAD_FILE_END (rebooting?), upload.totalSize= " << stHTTPUploadLocal.totalSize << endl;
  } //if(Update.end(true))
  else {
    Update.printError(Serial);
  } //if(Update.end(true))else
  Serial.setDebugOutput(false);
  return;
} //HandleOTAFileEnd


void PauseBlynk(void) {
    _bOTA_Started= true;
    Serial << LOG0 << " PauseBlynk(): Set _bOTA_Started to " << _bOTA_Started << endl;
    _ulUpdateTimeoutMsec= millis() + 20000;
  return;
} //PauseBlynk
#endif    //0
//Last line.
