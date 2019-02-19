//BeckE8266OTALib.cpp, Beck 1/16/19
#include <BeckMiniLib.h>
#include <BeckE8266OTALib.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char*         acServerIndex         = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
unsigned long       _ulUpdateTimeoutMsec   = 0;
bool                _bOTA_Started         = false;   //Turns off Blynk.
ESP8266WebServer    oESP8266WebServer(80);

//Function prototypes
void HandleOTAUpdate    (void);
void HandleOTAFileEnd   (HTTPUpload& stHTTPUploadLocal);
void PauseBlynk         (void);

void SetupOTAServer(const char *acHostname) {
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
    HandleOTAUpdate();
  });

  oESP8266WebServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial << LOG0 << "SetupOTAServer(): Open http://" << acHostname << ".local to perform an OTA update" << endl;
  //Serial << "SetupOTAServer(): Access this device using " << WiFi.localIP() << " or " << acHostname << ".local" << endl;
  return;
} //SetupOTAServer


void HandleOTAServer(void){
  oESP8266WebServer.handleClient();
  delay(1);
  return;
} //HandleOTAServer


void HandleOTAUpdate() {
  //upload() returns oHttpServer._currentUpload which is an HTTPUpload struct
  HTTPUpload& stHTTPUpload = oESP8266WebServer.upload();
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
//Last line.
