//BeckNTPLib.cpp, Beck 1/11/19
#include <BeckMiniLib.h>
#include <BeckNTPLib.h>
#include <NtpClientLib.h>   //Just for Eclipse resolving
#include <ESP8266WiFi.h>    //Just for Eclipse resolving
#include <Streaming.h>

String  szNtpServer       = "pool.ntp.org";
int     wTimeOffset       = 1;
bool    bDaylightSavings  = true;

//US Mountain Time Zone (Boise)
TimeChangeRule oMDT_Rule = {"MDT", Second, Sun, Mar, 2, -420};	//Mountain Daylight Time = UTC - 7 hours
TimeChangeRule oMST_Rule = {"MST", First , Sun, Nov, 2, -480};  //Mountain Standard Time = UTC - 8 hours

Timezone oMT_Timezone(oMDT_Rule, oMST_Rule);

TimeChangeRule *pTimeChangeRule;

//Function protos
void SetupNTPHandlers();

void SetupNTP(){
  Serial << LOG0 << "SetupNTP(): Begin" << endl;
  NTP.begin(szNtpServer, wTimeOffset, bDaylightSavings);
  NTP.setInterval(63);
  SetupNTPHandlers();
  return;
} //SetupNTP


void SetupNTPHandlers(){
  Serial << LOG0 << "SetupNTPHandlers(): Setup NTP.onNTPSyncEvent" << endl;
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
  if (ntpEvent) {
    //Serial.print("Time Sync error: ");
    Serial << LOG0 << "SetupNTPHandlers(): Time Sync error: ";
    if (ntpEvent == noResponse){
      //Serial.println("NTP server not reachable");
      Serial << "NTP server not reachable" << endl;
    } //if(ntpEvent==noResponse)
    else if (ntpEvent == invalidAddress){
      //Serial.println("Invalid NTP server address");
      Serial << "Invalid NTP server address" << endl;
    } //elseif(ntpEvent==invalidAddress)
  } //if(ntpEvent)
  else{
    Serial << LOG0 << "SetupNTPHandlers(): Got NTP time: " <<
              NTP.getTimeDateString(NTP.getLastNTPSync()) << endl;
    }   //if(ntpEvent)else
  }); //NTP.onNTPSyncEvent()
  return;
} //SetupNTPHandlers


String szPrintDigits(int digits) {
  // utility for digital clock display: prints preceding colon and leading 0
  String digStr = "";

  if (digits < 10)
    digStr += '0';
  digStr += String(digits);

  return digStr;
} //szPrintDigits


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
//Last line.
