//BeckWiFiLib.cpp, Beck Dec 10, 2018b
#include <BeckMiniLib.h>
#include <BeckWiFiLib.h>

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
  Serial << LOG0 << "SetupWiFi(): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  return;
} //SetupWiFi
//Last line.
