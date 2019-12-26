const char szSketchName[]  = "Beck_Stub.ino";
const char szFileDate[]    = "12/23/19a";

#ifndef ESP8266
  #define ESP8266
#endif

#define DO_ACCESS_POINT       true

//#include <BeckWiFiLib.h>
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif    //ESP8266
#include <Streaming.h>
//#include <Time.h>
//#include <WiFiClient.h>

#if DO_ACCESS_POINT
	#include <BeckAccessPointLib_Simple.h>
#endif

const long    	lSerialMonitorBaud  = 115200;

const char* 	WiFi_SSID 			= "Aspot24";
const char* 	WiFi_Password 		= "Qazqaz11";

const char* 	_acAccessPointSSID 		= "StubSpot";
const char* 	_acAccessPointPW 		= "Qazqaz11";

bool			_bWiFiConnected;


void setup(){
  Serial.begin(lSerialMonitorBaud);
  delay(100);
  Serial << endl  << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.begin(WiFi_SSID, WiFi_Password);
  //Serial.print("Connecting to Wi-Fi");
  Serial << "Setup(): Connecting to WiFi" << endl;
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }	//while
  _bWiFiConnected= true;
  Serial << endl << "Setup(:Connected with IP: " << WiFi.localIP() << endl;
#if DO_ACCESS_POINT
  SetupAccessPt(_acAccessPointSSID, _acAccessPointPW);
#endif  //DO_ACCESS_POINT

  Serial << "setup(): Done" << endl;
  return;
} //setup


void loop(){
  //ulLastTaskMsec= millis();
/*
  if (_bWiFiConnected){
    HandleWebServer();
    CheckTaskTime("loop(): HandleWebServer()");
  } //if(_bWiFiConnected)
*/
#if DO_ACCESS_POINT
  if (_bWiFiConnected){
    HandleSoftAPClient();       //Listen for HTTP requests from clients
  } //if(_bWiFiConnected)
#endif  //DO_ACCESS_POINT
  return;
} //loop
//Last line.
