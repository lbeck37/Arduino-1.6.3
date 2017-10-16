#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ESP32_HTTPUpdateServer.h>
//#include <Update_ESP32.h>
#include <Update.h>

const char* ESP32_HTTPUpdateServer::_serverIndex =
R"(<html><body><form method='POST' action='' enctype='multipart/form-data'>
                  <input type='file' name='update'>
                  <input type='submit' value='Update'>
               </form>
         </body></html>)";
const char* ESP32_HTTPUpdateServer::_failedResponse = R"(Update Failed!)";
const char* ESP32_HTTPUpdateServer::_successResponse = "<META http-equiv=\"refresh\" content=\"15;URL=\">Update Success! Rebooting...";


ESP32_HTTPUpdateServer::ESP32_HTTPUpdateServer(bool serial_debug)
{
  _serial_output = serial_debug;
  _pWebServer = NULL;
  _username = NULL;
  _password = NULL;
  _authenticated = false;
}	//constructor


void ESP32_HTTPUpdateServer::setup(WebServer *pWebServer, const char * path, const char * username, const char * password)
{
    _pWebServer = pWebServer;
    _username = (char *)username;
    _password = (char *)password;

    // handler for the /update form page
    _pWebServer->on(path, HTTP_GET, [&](){
      if(_username != NULL && _password != NULL && !_pWebServer->authenticate(_username, _password)){
        return _pWebServer->requestAuthentication();
      }	//if(_username!=NULL&&...
      _pWebServer->send(200, "text/html", _serverIndex);
    });

    // handler for the /update form POST (once file upload finishes)
    _pWebServer->on(path, HTTP_POST, [&](){
      if(!_authenticated){
        return _pWebServer->requestAuthentication();
      }	//if(!_authenticated)
      _pWebServer->send(200, "text/html", Update.hasError() ? _failedResponse : _successResponse);
      ESP.restart();
    },[&](){
      // handler for the file upload, get's the sketch bytes, and writes
      // them through the Update object

      HTTPUpload& stUpload = _pWebServer->upload();

			switch (stUpload.status) {
				case UPLOAD_FILE_START:
					if(_serial_output) Serial.setDebugOutput(true);
					_authenticated= (_username == NULL || _password == NULL || _pWebServer->authenticate(_username, _password));
					if(!_authenticated){
						if(_serial_output) Serial.printf("Unauthenticated Update\n");
						return;
					}	//if(!_authenticated)

					//WiFiUDP::stopAll();
					if (_serial_output) Serial.printf("Update: %s\n", stUpload.filename.c_str());

					//uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
					//if(!Update.begin(maxSketchSpace)){//start with max available size

					if(!Update.begin()){  //start with default
						if (_serial_output) Update.printError(Serial);
					}	//if(!Update.begin(maxSketchSpace))
					break;
				case UPLOAD_FILE_WRITE:
					if(_authenticated){
						if(_serial_output) Serial.printf(".");
						if(Update.write(stUpload.buf, stUpload.currentSize) != stUpload.currentSize){
							if(_serial_output) Update.printError(Serial);
						}	//if(Update.write(upload.buf,...
					}	//if(_authenticated)
					break;
				case UPLOAD_FILE_END:
					if(_authenticated){
						if(Update.end(true)){ //true to set the size to the current progress
							if(_serial_output) Serial.printf("Update Success: %u\nRebooting...\n", stUpload.totalSize);
						} //if(Update.end(true))
						else {
							if(_serial_output) Update.printError(Serial);
						}	//if(Update.end(true))else
						if(_serial_output) Serial.setDebugOutput(false);
					}	//if(_authenticated)
					break;
				case UPLOAD_FILE_ABORTED:
					if(_authenticated){
						Update.end();
						if(_serial_output) Serial.println("Update was aborted");
					}	//if(_authenticated)
					break;
				default:
					break;
			}	//switch
      delay(0);
    });	//if(upload.status==UPLOAD_FILE_START)
    return;
}	//setup
//Last line.
