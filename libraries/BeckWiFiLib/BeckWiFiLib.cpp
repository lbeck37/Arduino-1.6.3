//BeckWiFiLib.cpp, Beck Dec 15, 2018a
#include <BeckMiniLib.h>
#include <BeckWiFiLib.h>

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
	//IPAddress oIPAddress;

  Serial << LOG0 << "SetupWiFi()(BeckWiFiLib.cpp): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  //oIPAddress= WiFi.localIP();
  //Serial << LOG0 << "SetupWiFi(): oIPAddress: " << oIPAddress << endl;
  return;
} //SetupWiFi
//Last line.
