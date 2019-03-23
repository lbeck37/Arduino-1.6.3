//BeckWiFiLib.cpp, Beck March 23, 2019
#include <BeckWiFiLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
//#include <Streaming.h>

bool        _bWiFiConnected;

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
  uint32_t    ulWiFiWaitMsec      = 3 * lMsecPerSec; //mSec between running system handler
  uint32_t    ulWiFiTimeoutMsec;

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

  //Connect
  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

/*
  if (millis() >= ulNextThermHandlerMsec){
    ulNextThermHandlerMsec= millis() + ulThermHandlerPeriodMsec;
*/
/*
  //Wait for WL_CONNECTED
  while (WiFi.status() != WL_CONNECTED) {
    Serial << ".";
    delay(100);
  }
  Serial << endl;

  Serial << LOG0 << "SetupWiFi():  SSID= " << WiFi.SSID() << " IP address: " << WiFi.localIP() << endl;
*/
//Wait for WL_CONNECTED
  _bWiFiConnected= false;
  ulWiFiTimeoutMsec= millis() + ulWiFiWaitMsec;
  while (!_bWiFiConnected && millis() <= ulWiFiTimeoutMsec){
    if (WiFi.status() == WL_CONNECTED){
      _bWiFiConnected= true;
    } //if(WiFi.status()!=WL_CONNECTED)
    else {
      Serial << ".";
      delay(100);
    } //if(WiFi.status()!=WL_CONNECTED)else
  } //while(!_bWiFiConnected&&millis()<=ulWiFiTimeoutMsec)
  Serial << endl;
  if (_bWiFiConnected){
    Serial << LOG0 << "SetupWiFi():  SSID= " << WiFi.SSID() << " IP address: " << WiFi.localIP() << endl;
  } //if(bWiFiConnected)
  else{
    Serial << LOG0 << "SetupWiFi(): WiFi failed to connect." << endl;
  } //if(bWiFiConnected)else
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
