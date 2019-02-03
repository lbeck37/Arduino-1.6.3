//BeckNTPLib.cpp, Beck 2/2/19
#include <BeckMiniLib.h>
#include <BeckNTPLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

int             wMtnTimeHourDifference  = 8;
int             wMsecPerMinute          = 60 *1000;
int             wSecPerHour             = 3600;
long            lSecTimeOffset          = wMtnTimeHourDifference * wSecPerHour;
unsigned long   ulMsecUpdateInterval    = 1 * wMsecPerMinute;
char*           szNtpServer             = "pool.ntp.org";

WiFiUDP       oUDPforNTP;
NTPClient     oNTPClient= NTPClient(oUDPforNTP, szNtpServer, lSecTimeOffset, ulMsecUpdateInterval);


void SetupNTP(){
  Serial << LOG0 << "BeckNTPLib: SetupNTP(): Call oNTPClient.begin()" << endl;
  oNTPClient.begin();
  return;
} //SetupNTP


void HandleNTPUpdate(void){
  oNTPClient.update();
  return;
} //HandleNTPUpdate
//Last line.
