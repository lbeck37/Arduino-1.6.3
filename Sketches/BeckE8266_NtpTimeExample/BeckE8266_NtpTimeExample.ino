const char szSketchName[]  = "BeckE8266_NtpTimeExample.ino";
const char szFileDate[]    = "Lenny 12/10/18p";
/*
 Name:    NtpClient.ino
 Created: 20/08/2016
 Author:  gmag11@gmail.com
 Editor:  http://www.visualmicro.com
*/
#include <BeckMiniLib.h>
#include <BeckOTALib.h>
#include <BeckNtpLib.h>
#include <NtpClientLib.h>   //Just for Eclipse resolving
#include <ESP8266WiFi.h>    //Just for Eclipse resolving
#include <Streaming.h>      //Just for Eclipse resolving
#include <TimeLib.h>
#include <Timezone.h>

static const char   	szRouterName[]        = "Aspot24";
static const char   	szRouterPW[]          = "Qazqaz11";

static const char   acHostname[]			= "BeckThermoDev";
static const int		wLoopMilliDelay		= 5100;

void setup(){
  static WiFiEventHandler 	e1;
  static WiFiEventHandler 	e2;

  Serial.begin(115200);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  WiFi.mode(WIFI_STA);
  WiFi.begin(szRouterName, szRouterPW);

  SetupOTAServer(acHostname);
  SetupNTP();

  WiFi.onEvent([](WiFiEvent_t oWiFiEvent) {
    Serial.printf("WiFi event, oWiFiEvent= %d\n", oWiFiEvent);
  });
  e1= WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start NTP Client
  e2= WiFi.onStationModeDisconnected(onSTADisconnected);
  return;
} //setup


void loop(){
  static unsigned long ulLastMilli = 0;

	HandleOTAServer();
  if ((millis() - ulLastMilli) > wLoopMilliDelay) {
    ulLastMilli = millis();
    Serial << LOG0 << "loop(): Corrected: " << szFormatDateString() << ", "
    		<< szFormatTimeString() << endl;
  }	//if((millis()-ulLastMilli)>sLoopMilliDelay)
  delay(0);
  return;
}	//loop
//Last line.
