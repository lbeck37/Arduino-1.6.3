static const char szSketchName[]  = "BeckWebUpdate.ino";
static const char szFileDate[]    = "Jan 10, 2016H";
//1/10/16 Ported from C:\Dev\_Repos\ESP8266\Arduino\libraries\ESP8266WebServer\examples\WebUpdate\WebUpdate.ino
//To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* host = "esp37";
const char* ssid = "Aspot24";
const char* password = "Qazqaz11";

ESP8266WebServer server(80);
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

void setup(void){
  SetupServer();
  return;
} //setup


void SetupServer(void) {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  Serial.printf("setup(): Sketch: %s, %s\n", szSketchName, szFileDate);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(host);
    server.on("/", HTTP_GET, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        uint32_t ulMaxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        Serial.printf("SetupServer(): maxSketchSpace= %u\n", ulMaxSketchSpace);
        Serial.printf("SetupServer(): Update filename: %s\n", upload.filename.c_str());
        if(!Update.begin(ulMaxSketchSpace)) {//start with max available size
          Update.printError(Serial);
        }	//if(!Update.begin(maxSketchSpace))
      }	//if(WiFi.waitForConnectResult()==WL_CONNECTED)
      else if(upload.status == UPLOAD_FILE_WRITE) {
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }	//if(Update.write(upload.buf, upload.currentSize) != upload.currentSize)
      }	//else if(upload.status==UPLOAD_FILE_WRITE)
      else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        }	//if(Update.end(true))
        else {
          Update.printError(Serial);
        }	//if(Update.end(true))else
        Serial.setDebugOutput(false);
      }	//else if(upload.status==UPLOAD_FILE_END)
      yield();
    });
    server.begin();
    MDNS.addService("http", "tcp", 80);

    Serial.printf("Ready! Open http://%s.local in your browser to perform an OTA update\n", host);
  }	//if(WiFi.waitForConnectResult()==WL_CONNECTED)
  else {
    Serial.println("WiFi Failed");
  }	//if(WiFi.waitForConnectResult()==WL_CONNECTED)else
  return;
} //SetupServer


void loop(void){
  HandleClient();
  return;
} //loop


void HandleClient(void){
  server.handleClient();
  delay(1);
  return;
} //HandleClient
//Last line.
