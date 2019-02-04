//BeckNTPLib.cpp, Beck 2/3/19c
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
int             wMinuteUpdateInterval   = 1;
unsigned long   ulMsecUpdateInterval    = wMinuteUpdateInterval * wMsecPerMinute;
char*           szNtpServer             = "pool.ntp.org";

WiFiUDP       oUDPforNTP;
NTPClient     oNTPClient= NTPClient(oUDPforNTP, szNtpServer);


void SetupNTP(){
  int     wCount    = 0;
  bool    bResult   = false;
  Serial << LOG0 << "BeckNTPLib: SetupNTP(): Start NTP, Update= " << wMinuteUpdateInterval <<
      " minutes, Offset= " << wMtnTimeHourDifference << " hour" << endl;
  oNTPClient.begin();
  oNTPClient.setUpdateInterval  (ulMsecUpdateInterval);
  oNTPClient.setTimeOffset      (lSecTimeOffset);
  //Try up to 5 times to get the network time
  while (!bResult && (wCount++ < 5)){
    Serial << LOG0 << "BeckNTPLib: SetupNTP() Call  oNTPClient.forceUpdate()" << endl;
    bResult= oNTPClient.forceUpdate();
    delay(100);
  } //while
  unsigned long ulLocalTime= oNTPClient.getEpochTime();
  Serial << LOG0 << "BeckNTPLib: SetupNTP() Return, Date= " << szFormatDateString(ulLocalTime) << endl;
  return;
} //SetupNTP


void HandleNTPUpdate(void){
  oNTPClient.update();
  return;
} //HandleNTPUpdate
//Last line.
