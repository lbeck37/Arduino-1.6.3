const String szSketchName  = "BeckAngularJS_Test.ino";
const String szFileDate    = "April 16, 2019m";
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/
#include <BeckMiniLib.h>
#include <BeckOTALib.h>
#include <BeckWebServer.h>
#include <BeckWiFiLib.h>

const   char        _acHostname[]       = "Beckboard";
const   uint32_t    ulLogPeriodMsec     = 10 * lMsecPerSec; //mSec between running system handler
        uint32_t    ulNextLogMsec       = 0;
        int         wLogCount           = 0;

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  SetupWiFi();
  if (_bWiFiConnected){
    //SetupOTAWebPages();
    StartWebServer(_acHostname);
    Serial << LOG0 << "setup(): Open http://" << WiFi.localIP() << "/login to perform an OTA update" << endl;
  } //if(_bWiFiConnected)
  else{
    while(true){
      Serial << LOG0 << "setup(): WiFi failed to connect" << endl;
      delay(10000); //10 sec
     }  //while(true)
  } //if(_bSystemOk)else
  Serial << LOG0 << "setup(): Done" << endl;
  return;
} //setup


void loop(){
  HandleWebServer();
  if (millis() >= ulNextLogMsec){
    ulNextLogMsec= millis() + ulLogPeriodMsec;
    Serial << LOG0 << "loop(): Log number " << ++wLogCount << endl;
  } //if(millis()>=ulNextLogMsec)
  return;
} //loop
//Last line
