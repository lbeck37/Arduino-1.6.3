//BeckWiFiLib.cpp, Beck Jan 23, 2019
#include <BeckMiniLib.h>
#include <BeckWiFiLib.h>
#include <Streaming.h>

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
/*
  Serial << LOG0 << "SetupWiFi()(BeckWiFiLib.cpp): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  //Reports 0.0.0
  Serial << LOG0 << "SetupWiFi(): Local IP: " << WiFi.localIP() << endl;
*/
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  //Serial << LOG0 << "[WIFI] Connecting to ", szRouterName << endl;
  //WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    delay(100);
  }
  Serial << endl;

  // Connected!
  //Serial << LOG0 << "[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str()) << endl;
  //Serial << LOG0 << "[WIFI] STATION Mode, SSID: " << WiFi.SSID().c_str() << " IP address: " << WiFi.localIP().toString().c_str()) << endl;
  Serial << LOG0 << "SetupWiFi():  SSID= " << WiFi.SSID() << " IP address: " << WiFi.localIP() << endl;
  return;
} //SetupWiFi
//Last line.
