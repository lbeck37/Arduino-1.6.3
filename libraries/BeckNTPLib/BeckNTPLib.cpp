//BeckNTPLib.cpp, Beck 12/10/18
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

void SetupNTP(){
  static WiFiEventHandler 	oEventHandler1;
  static WiFiEventHandler 	oEventHandler2;

  Serial << LOG0 << "SetupNTP(): Setup NTP.onNTPSyncEvent" << endl;
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
	if (ntpEvent) {
		Serial.print("Time Sync error: ");
		if (ntpEvent == noResponse)
			Serial.println("NTP server not reachable");
		else if (ntpEvent == invalidAddress)
			Serial.println("Invalid NTP server address");
	}	//if (ntpEvent)
  else{
  	Serial << LOG0 << "SetupNTP(): Got NTP time: " <<
              NTP.getTimeDateString(NTP.getLastNTPSync()) << endl;
    }
  });

  Serial << LOG0 << "SetupNTP(): Setup WiFi.onEvent" << endl;
  WiFi.onEvent([](WiFiEvent_t oEvent) {
    Serial.printf("Event wifi -----> %d\n", oEvent);
 });

  Serial << LOG0 << "SetupNTP(): Setup WiFi.onStationModeGotIP" << endl;
  oEventHandler1= WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start NTP Client

  Serial << LOG0 << "SetupNTP(): Setup WiFi.onStationModeDisconnected" << endl;
  oEventHandler2= WiFi.onStationModeDisconnected(onSTADisconnected);
  return;
} //SetupNTP


void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
  //NTP.begin("pool.ntp.org", 1, true);
  NTP.begin(szNtpServer, wTimeOffset, bDaylightSavings);
  NTP.setInterval(63);
  digitalWrite(2, LOW);
  return;
} //onSTAGotIP


void onSTADisconnected(WiFiEventStationModeDisconnected event_info) {
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
  digitalWrite(2, HIGH);
  return;
} //onSTADisconnected


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

/*Old code
String szGetTime(long lMsec){
  String  szString;

  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";     //Send with trailing blank to seperate from next field.
  return szString;
} //szGetTime
*/
//Last line.
