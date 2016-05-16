#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "ESP8266HTTPUpdateServer.h"

#include <BeckLib.h>	//Beck 5/13/16

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
      _server->sendHeader("Connection", "close");
      _server->sendHeader("Access-Control-Allow-Origin", "*");
      _server->send(200, "text/html", _serverIndex);
      BLog("ESP8266HTTPUpdateServer::setup(): HTTP_GET");
    });

    // handler for the /update form POST (once file upload finishes)
    _server->on("/update", HTTP_POST, [&](){
      _server->sendHeader("Connection", "close");
      _server->sendHeader("Access-Control-Allow-Origin", "*");
      _server->send(200, "text/html", (Update.hasError())?"FAIL":"<META http-equiv=\"refresh\" content=\"15;URL=/update\">OK");
      BLog("ESP8266HTTPUpdateServer::setup(): HTTP_POST, calling ESP.restart()");
      ESP.restart();
    },[&](){
      // handler for the file upload, get's the sketch bytes, and writes
      // them through the Update object
      HTTPUpload& upload = _server->upload();
      if(upload.status == UPLOAD_FILE_START){
        BLog("ESP8266HTTPUpdateServer::setup(): UPLOAD_FILE_START");
        if (_serial_output) {
        	Serial.setDebugOutput(true);
        }	//if(_serial_output)
        WiFiUDP::stopAll();

        if (_serial_output){
            //Serial.printf("Update: %s\n", upload.filename.c_str());
        }	//if(_serial_output)
        String sFilename= upload.filename.c_str();
        BLog("setup(): Update filename: " + sFilename);

        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        BLog("setup(): maxSketchSpace= " + (String)maxSketchSpace);

        if(!Update.begin(maxSketchSpace)) {      //start with max available size
          if (_serial_output) {
        	  Update.printError(Serial);
          }	//if(_serial_output)
        }	//if(!Update.begin(maxSketchSpace))
      }  //if(upload.status == UPLOAD_FILE_START)
      else if(upload.status == UPLOAD_FILE_WRITE) {
        if (_serial_output) {
        	Serial.printf(".");
        	if((++wHttpServerCount % 50) == 1) {
            Serial.printf("\n");
        		BLogS("setup(): UPLOAD_FILE_WRITE, wHttpServerCount= " + String(wHttpServerCount));
        		BLogS("setup(): upload.totalSize  = " + String(upload.totalSize));
        		BLogS("setup(): upload.currentSize= " + String(upload.currentSize));
        	}	//if((++wHttpServerCount%10)==1)
        }	//if (_serial_output)

        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          if (_serial_output) {
        	  Update.printError(Serial);
          }	//if(_serial_output)
          BLogS("setup(): Update.write() ERROR, upload.currentSize= " + String(upload.currentSize));
        } //if(Update.write(upload.buf,...
      } //else if(upload.status==UPLOAD_FILE_WRITE)
      else if(upload.status == UPLOAD_FILE_END) {
        if(Update.end(true)) { //true to set the size to the current progress
          Serial.printf("\n");
          BLogS("setup(): UPLOAD_FILE_END, upload.totalSize= " + String(upload.totalSize));
          if (_serial_output) {
        	  Serial.printf("Update Success: upload.totalSize= %u\nRebooting\n", upload.totalSize);
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
        BLog("ESP8266HTTPUpdateServer::setup(): UPLOAD_FILE_END, upload.totalSize= " + String(upload.totalSize));
      }	//else if(upload.status==UPLOAD_FILE_ABORTED)
      delay(0);
    });
}	//setup
//Last line
