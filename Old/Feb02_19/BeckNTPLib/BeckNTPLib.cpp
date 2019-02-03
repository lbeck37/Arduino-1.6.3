//BeckNTPLib.cpp, Beck 1/27/19
#include <BeckMiniLib.h>
#include <BeckNTPLib.h>

#include <NtpClientLib.h>
//#include <NtpClient.h>
//#include <TimeLib.h>
#include <Timezone.h>

#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

String  szNtpServer       = "pool.ntp.org";
int     wTimeOffset       = 1;
bool    bDaylightSavings  = true;

//US Mountain Time Zone (Boise)
TimeChangeRule oMDT_Rule = {"MDT", Second, Sun, Mar, 2, -420};	//Mountain Daylight Time = UTC - 7 hours
TimeChangeRule oMST_Rule = {"MST", First , Sun, Nov, 2, -480};  //Mountain Standard Time = UTC - 8 hours

Timezone          oMT_Timezone(oMDT_Rule, oMST_Rule);
TimeChangeRule*   pTimeChangeRule;

//Local function prototypes
void SetupNTPHandlers();

void SetupNTP(){
  Serial << LOG0 << "SetupNTP(): Call NTP.begin for |" << szNtpServer << "|" << endl;
  NTP.begin(szNtpServer, wTimeOffset, bDaylightSavings);
  //NTP.setInterval(63);
  NTP.setInterval(12);
  SetupNTPHandlers();
  return;
} //SetupNTP


/*
void SetupNTPHandlers(){
  Serial << LOG0 << "BeckNTPLib: SetupNTPHandlers(): Create NTP.onNTPSyncEvent inline handler" << endl;
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent) {
      Serial << LOG0 << "SetupNTPHandlers(): Time Sync error: ";
      if (ntpEvent == noResponse){
        Serial << "NTP server not reachable" << endl;
      } //if(ntpEvent==noResponse)
      else if (ntpEvent == invalidAddress){
        Serial << "Invalid NTP server address" << endl;
      } //elseif(ntpEvent==invalidAddress)
    } //if(ntpEvent)
    else{
      Serial << LOG0 << "BeckNTPLib: SetupNTPHandlers(): onNTPSyncEvent: Got NTP time: " <<
         NTP.getTimeDateString(NTP.getLastNTPSync()) << endl;
      }   //if(ntpEvent)else
  }); //NTP.onNTPSyncEvent()
  return;
} //SetupNTPHandlers
*/
void SetupNTPHandlers(){
  Serial << LOG0 << "BeckNTPLib: SetupNTPHandlers(): Create NTP.onNTPSyncEvent inline handler" << endl;
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (ntpEvent != timeSyncd) {
      Serial << LOG0 << "SetupNTPHandlers(): Time Sync error: ";
      if (ntpEvent == noResponse){
        Serial << "NTP server not reachable" << endl;
      } //if(ntpEvent==noResponse)
      else if (ntpEvent == invalidAddress){
        Serial << "Invalid NTP server address" << endl;
      } //elseif(ntpEvent==invalidAddress)
    } //if(ntpEvent!=timeSyncd)
    else{
      Serial << LOG0 << "BeckNTPLib: SetupNTPHandlers(): onNTPSyncEvent: Got NTP time: " <<
         NTP.getTimeDateString(NTP.getLastNTPSync()) << endl;
      }   //if(ntpEvent!=timeSyncd)else
  }); //NTP.onNTPSyncEvent()
  return;
} //SetupNTPHandlers


/*
String szPrintDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  String digStr = "";

  if (digits < 10)
    digStr += '0';
  digStr += String(digits);

  return digStr;
} //szPrintDigits
*/


/*
String szFormatTimeString(void) {
	time_t		lBoiseTime= oMT_Timezone.toLocal (now(), &pTimeChangeRule);

	String szReturnString = "";
	szReturnString += szPrintDigits(hour(lBoiseTime));
	szReturnString += ":";
	szReturnString += szPrintDigits(minute(lBoiseTime));
	szReturnString += ":";
	szReturnString += szPrintDigits(second(lBoiseTime));
	szReturnString += " ";
	return szReturnString;
} //szFormatTimeString


String szFormatDateString(void){
	time_t		lBoiseTime= oMT_Timezone.toLocal (now(), &pTimeChangeRule);

  String szReturnString = "";
  szReturnString += szPrintDigits(month(lBoiseTime));
  szReturnString += "/";
  szReturnString += szPrintDigits(day(lBoiseTime));
  szReturnString += "/";
  szReturnString += String(year(lBoiseTime));
  return szReturnString;
} //szFormatDateString
*/
//#endif  //not ESP32
//Last line.
