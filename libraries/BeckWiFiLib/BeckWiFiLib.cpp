//BeckWiFiLib.cpp, Beck Dec 10, 2018b
#include <BeckMiniLib.h>
#include <BeckWiFiLib.h>

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
#if false
  Serial << LOG0 << "SetupWiFi(): Call WiFi.mode(WIFI_AP_STA)" << endl;
  WiFi.mode(WIFI_AP_STA);
#else
  Serial << LOG0 << "SetupWiFi(): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);
#endif

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

#if false
  //wl_status_t eWiFiStatus= WiFi.waitForConnectResult();
  Serial << LOG0 << "SetupWiFi(): Call WiFi.waitForConnectResult()" << endl;
  wl_status_t eWiFiStatus= (wl_status_t)WiFi.waitForConnectResult();

  if(eWiFiStatus == WL_CONNECTED) {
    Serial << LOG0 << "SetupWiFi(): WiFi.waitForConnectResult() returned " << szWiFiStatus(eWiFiStatus) << endl;
    //Serial.printf("\nHTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
    Serial << LOG0 << "SetupWiFi(): IP address= " << WiFi.localIP() << endl;
  } //if(eWiFiStatus==WL_CONNECTED)
  else {
    //Serial << LOG0 << " SetupServer(): ERROR: WiFi.waitForConnectResult() returned " << ucWiFiStatus << endl;
    Serial << LOG0 << "SetupWiFi(): ERROR: WiFi.waitForConnectResult() returned " << szWiFiStatus(eWiFiStatus) << endl;
  } //if(eWiFiStatus==WL_CONNECTED)else
#endif
  return;
} //SetupWiFi


String szWiFiStatus(wl_status_t eWiFiStatus){
  String szStatus;
  switch (eWiFiStatus){
  case WL_IDLE_STATUS:
    szStatus= "WL_IDLE_STATUS";
    break;
  case WL_NO_SSID_AVAIL:
    szStatus= "WL_NO_SSID_AVAIL";
    break;
  case WL_SCAN_COMPLETED:
    szStatus= "WL_SCAN_COMPLETED";
    break;
  case WL_CONNECTED:
    szStatus= "WL_CONNECTED";
    break;
  case WL_CONNECT_FAILED:
    szStatus= "WL_CONNECT_FAILED";
    break;
  case WL_CONNECTION_LOST:
    szStatus= "WL_CONNECTION_LOST";
    break;
  case WL_DISCONNECTED:
    szStatus= "WL_DISCONNECTED";
    break;
  case WL_NO_SHIELD:
    szStatus= "WL_NO_SHIELD";
    break;
  default:
    szStatus= "default";
    break;
  } //switch
  return szStatus;
} //szWiFiStatus
//Last line.
