const char szSketchName[]  = "BeckE8266_NtpTimeExample.ino";
const char szFileDate[]    = "Lenny 12/07/18g";
/*
 Name:    NtpClient.ino
 Created: 20/08/2016
 Author:  gmag11@gmail.com
 Editor:  http://www.visualmicro.com
*/
#include <BeckNtpLib.h>
#include <NtpClientLib.h>   //Just for Eclipse resolving
#include <ESP8266WiFi.h>    //Just for Eclipse resolving
#include <Streaming.h>      //Just for Eclipse resolving
#include <BeckMiniLib.h>
#include <TimeLib.h>
#include <stdio.h>
#include <time.h>

#ifndef WIFI_CONFIG_H
#define YOUR_WIFI_SSID "Aspot24"
#define YOUR_WIFI_PASSWD "Qazqaz11"
#endif // !WIFI_CONFIG_H

void setup(){
  static WiFiEventHandler 	e1;
  static WiFiEventHandler 	e2;

  Serial.begin(115200);
  Serial << endl << LOG0 << "setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;
  Serial << LOG0 << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.mode(WIFI_STA);
  WiFi.begin(YOUR_WIFI_SSID, YOUR_WIFI_PASSWD);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  int		wTimeZone= -7;
  NTP.setTimeZone(wTimeZone);

  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
      Serial.print("Time Sync error: ");
      if (ntpEvent == noResponse)
        Serial.println("NTP server not reachable");
      else if (ntpEvent == invalidAddress)
        Serial.println("Invalid NTP server address");
    }
    else {
      Serial.print("Got NTP time: ");
      Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
    }
  });
  WiFi.onEvent([](WiFiEvent_t e) {
    Serial.printf("Event wifi -----> %d\n", e);
  });
  e1= WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start NTP Client
  e2= WiFi.onStationModeDisconnected(onSTADisconnected);
  return;
} //setup


void loop(){
  static int i 		= 0;
  static int last = 0;

  if ((millis() - last) > 5100) {
    //Serial.println(millis() - last);
    last = millis();
    Serial.print(i); Serial.print(" ");
    Serial.print(NTP.getTimeDateString()); Serial.print(" ");
    Serial.print(NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ");
    Serial.print("WiFi is ");
    Serial.print(WiFi.isConnected() ? "connected" : "not connected"); Serial.print(". ");
    Serial.print("Uptime: ");
    Serial.print(NTP.getUptimeString()); Serial.print(" since ");
    Serial.println(NTP.getTimeDateString(NTP.getFirstSync()).c_str());
    i++;

/*
    time_t 				lTimeInput;
    struct tm 		*pTimeInfo;
    char 					acBuffer[80];
    tmElements_t 	stTimeElements;
*/
    //lTimeInput= now();
    Serial << LOG0 << "loop(): " << szFormatDateString(now()) << ", "
    		<< szFormatTimeString(now()) << endl;
}	//if((millis()-last)>5100)
  delay(0);
  return;
}	//loop
//Last line.
