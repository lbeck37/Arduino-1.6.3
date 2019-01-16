//BeckWiFiLib.cpp, Beck Jan 16, 2019
#include <BeckMiniLib.h>
#include <BeckWiFiLib.h>

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
  Serial << LOG0 << "SetupWiFi()(BeckWiFiLib.cpp): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  //Reports 0.0.0
  //Serial << LOG0 << "SetupWiFi(): Local IP: " << WiFi.localIP() << endl;
  return;
} //SetupWiFi
//Last line.
