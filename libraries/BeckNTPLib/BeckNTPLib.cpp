#include <BeckNTPLib.h>

String 	szNtpServer 			= "pool.ntp.org";
int			wTimeOffset				= 1;
bool		bDaylightSavings	= true;

void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) {
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
  //NTP.begin("pool.ntp.org", 1, true);
  NTP.begin(szNtpServer, wTimeOffset, bDaylightSavings);
  NTP.setInterval(63);
  digitalWrite(2, LOW);
}

void onSTADisconnected(WiFiEventStationModeDisconnected event_info) {
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
  digitalWrite(2, HIGH);
}
