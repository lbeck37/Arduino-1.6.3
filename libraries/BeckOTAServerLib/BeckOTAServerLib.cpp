//BeckOTAServerLib.cpp
// Jan 30, 2017 Beck
//This is update code that was in BeckBlynkESP.ino

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char*     acServerIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

ESP8266WebServer    oESP8266WebServer(80);

//Function protos
void SetupServer(void);
void HandleUpdate();
void HandleFileEnd(HTTPUpload& stHTTPUploadLocal);
void PauseBlynk();
void HandleHttpServer(void);

//Functions
void SetupServer(void) {
    MDNS.begin(acHostname);
    oESP8266WebServer.on("/", HTTP_GET, [](){
      oESP8266WebServer.sendHeader("Connection", "close");
      oESP8266WebServer.sendHeader("Access-Control-Allow-Origin", "*");
      oESP8266WebServer.send(200, "text/html", acServerIndex);
    });
    oESP8266WebServer.on("/update", HTTP_POST, []() {
      oESP8266WebServer.sendHeader("Connection", "close");
      oESP8266WebServer.sendHeader("Access-Control-Allow-Origin", "*");
      oESP8266WebServer.send(200, "text/plain", (Update.hasError()) ? "Update Failed!" : "Update Successful!");
      ESP.restart();
    },[](){
      HandleUpdate();
    });
    oESP8266WebServer.begin();
    MDNS.addService("http", "tcp", 80);
    Serial << LOG0 << " SetupServer(): Open http://" << acHostname << ".local to perform an OTA update" << endl;
  return;
} //SetupServer


void HandleUpdate() {
  //upload() returns oHttpServer._currentUpload which is an HTTPUpload struct
  HTTPUpload& stHTTPUpload = oESP8266WebServer.upload();
  if (stHTTPUpload.status == UPLOAD_FILE_START) {
    PauseBlynk();
    Serial.setDebugOutput(true);
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
    HandleFileEnd(stHTTPUpload);
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
#endif
  } //else if(upload.status==UPLOAD_FILE_END)
  yield();
  return;
} //HandleUpdate


void HandleFileEnd(HTTPUpload& stHTTPUploadLocal) {
  if (Update.end(true)) { //true to set the size to the current progress
    Serial << LOG0 << " HandleFileEnd(): UPLOAD_FILE_END (rebooting?), upload.totalSize= " << stHTTPUploadLocal.totalSize << endl;
  } //if(Update.end(true))
  else {
    Update.printError(Serial);
  } //if(Update.end(true))else
  Serial.setDebugOutput(false);
  return;
} //HandleFileEnd


void PauseBlynk() {
    bUpdating= true;
    Serial << LOG0 << " PauseBlynk(): Set bUpdating to " << bUpdating << endl;
    ulUpdateTimeoutMsec= millis() + 20000;
  return;
} //PauseBlynk
//#ifndef SKIP_SERVER


void HandleHttpServer(void){
  oESP8266WebServer.handleClient();
  delay(1);
  return;
} //HandleHttpServer
//Last line.