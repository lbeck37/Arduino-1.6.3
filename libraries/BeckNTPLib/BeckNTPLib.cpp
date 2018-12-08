//Beck 12/07/18 BeckNTPLib.cpp
#include <BeckNTPLib.h>
#include <NtpClientLib.h>   //Just for Eclipse resolving
#include <ESP8266WiFi.h>    //Just for Eclipse resolving

String  szNtpServer       = "pool.ntp.org";
int     wTimeOffset       = 1;
bool    bDaylightSavings  = true;

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
}	//szPrintDigits


String szFormatTimeString(time_t lCurrentTime) {
		String szReturnString = "";
		szReturnString += szPrintDigits(hour(lCurrentTime));
		szReturnString += ":";
		szReturnString += szPrintDigits(minute(lCurrentTime));
		szReturnString += ":";
		szReturnString += szPrintDigits(second(lCurrentTime));

		return szReturnString;
}	//szFormatTimeString


String szFormatDateString(time_t lCurrentTime){
	String szReturnString = "";
	//szReturnString += szPrintDigits(day(lCurrentTime));
	szReturnString += szPrintDigits(month(lCurrentTime));
	szReturnString += "/";
	//szReturnString += szPrintDigits(month(lCurrentTime));
	szReturnString += szPrintDigits(day(lCurrentTime));
	szReturnString += "/";
	szReturnString += String(year(lCurrentTime));
	return szReturnString;
}	//szFormatDateString

//Last line.
