const char szSketchName[]  = "BeckE8266_NtpTimeExample.ino";
const char szFileDate[]    = "Lenny 12/10/18t";
/*
 Name:    NtpClient.ino
 Created: 20/08/2016
 Author:  gmag11@gmail.com
 Editor:  http://www.visualmicro.com
*/
#include <BeckMiniLib.h>
//#include <BeckWiFiLib.h>
#include <BeckBlynkLib.h>
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
  Serial.begin(115200);
  Serial << endl << LOG0 << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << LOG0 << "setup(): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);

  Serial << LOG0 << "setup(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

  SetupOTAServer(acHostname);
  SetupNTP();
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
