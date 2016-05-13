#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "ESP8266HTTPUpdateServer.h"

#include <BeckLib.h>

const char* ESP8266HTTPUpdateServer::_serverIndex =
R"(<html><body><form method='POST' action='/update' enctype='multipart/form-data'>
                  <input type='file' name='update'>
                  <input type='submit' value='Update'>
               </form>
         </body></html>)";

ESP8266HTTPUpdateServer::ESP8266HTTPUpdateServer(bool serial_debug)
{
  _serial_output = serial_debug;
  _server = NULL;
}

void ESP8266HTTPUpdateServer::setup(ESP8266WebServer *server)
{
    _server = server;

    // handler for the /update form page
    _server->on("/update", HTTP_GET, [&](){
      pBeckFBase->LogToBoth("ESP8266HTTPUpdateServer::setup(): HTTP_GET");	//Beck 5/12/16
      _server->sendHeader("Connection", "close");
      _server->sendHeader("Access-Control-Allow-Origin", "*");
      _server->send(200, "text/html", _serverIndex);
    });

    // handler for the /update form POST (once file upload finishes)
    _server->on("/update", HTTP_POST, [&](){
        pBeckFBase->LogToBoth("ESP8266HTTPUpdateServer::setup(): HTTP_POST");	//Beck 5/12/16
      _server->sendHeader("Connection", "close");
      _server->sendHeader("Access-Control-Allow-Origin", "*");
      _server->send(200, "text/html", (Update.hasError())?"FAIL":"<META http-equiv=\"refresh\" content=\"15;URL=/update\">OK");
      ESP.restart();
    },[&](){
      // handler for the file upload, get's the sketch bytes, and writes
      // them through the Update object
      HTTPUpload& upload = _server->upload();
      if(upload.status == UPLOAD_FILE_START){
        pBeckFBase->LogToBoth("ESP8266HTTPUpdateServer::setup(): UPLOAD_FILE_START");	//Beck 5/12/16
        if (_serial_output) {
        	Serial.setDebugOutput(true);
        }	//if(_serial_output)
        WiFiUDP::stopAll();

        if (_serial_output){
            //Serial.printf("Update: %s\n", upload.filename.c_str());
        }	//if(_serial_output)
        String sFilename= upload.filename.c_str();
        pBeckFBase->LogToBoth("ESP8266HTTPUpdateServer::setup(): Update: " + sFilename); //Beck 5/12/16

        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        pBeckFBase->LogToBoth("ESP8266HTTPUpdateServer::setup(): maxSketchSpace= " + (String)maxSketchSpace); //Beck 5/12/16

        if(!Update.begin(maxSketchSpace)) {      //start with max available size
          if (_serial_output) {
        	  Update.printError(Serial);
          }	//if(_serial_output)
        }	//if(!Update.begin(maxSketchSpace))
      }  //if(upload.status == UPLOAD_FILE_START)
      else if(upload.status == UPLOAD_FILE_WRITE) {
        if (_serial_output) {
        	Serial.printf(".");
        }	//if (_serial_output)

        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          if (_serial_output) {
        	  Update.printError(Serial);
          }	//if(_serial_output)
        } //if(Update.write(upload.buf,...
      } //else if(upload.status==UPLOAD_FILE_WRITE)
      else if(upload.status == UPLOAD_FILE_END) {
        if(Update.end(true)) { //true to set the size to the current progress
          if (_serial_output) {
        	  Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          }	//if(_serial_output)
        } 	//if(Update.end(true))
        else {
          if (_serial_output) {
        	  Update.printError(Serial);
          }	//if (_serial_output)
        } 	//if(Update.end(true))else

        if (_serial_output) {
        	Serial.setDebugOutput(false);
        }	//if(_serial_output)
      } 	//else if(upload.status==UPLOAD_FILE_END)

      else if(upload.status == UPLOAD_FILE_ABORTED){
        Update.end();
        if (_serial_output) {
        	Serial.println("Update was aborted");
        }	//if(_serial_output)
      }	//else if(upload.status==UPLOAD_FILE_ABORTED)
      delay(0);
    });
}	//setup
//Last line
