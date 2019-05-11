//BeckWiFiLib.cpp, 5/3/19a
#include <BeckWiFiLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <ESP8266WiFiMulti.h>
#ifdef ESP8266
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti    oWiFiMulti;
#else
  #include <WiFiMulti.h>
  WiFiMulti           oWiFiMulti;
#endif    //ESP8266

const int     _wSSIDNumChar   = 32;
const int     _wPWNumChar     = 65;
const int     _wNumRouters    =  3;
char          _acRouterNames     [_wNumRouters][_wSSIDNumChar] = {"Aspot24" , "Lspot"   , "Cspot"};
//char          _acRouterNames     [_wNumRouters][_wSSIDNumChar] = {"Cspot"   , "Aspot24" , "Lspot"};
//char          _acRouterNames     [_wNumRouters][_wSSIDNumChar] = {"Lspot"   , "Aspot24" , "Cspot"};
//char          _acRouterNames     [_wNumRouters][_wSSIDNumChar] = {"Cspot"   , "Lspot"   , "Aspot24"};
char          _acRouterPWs       [_wNumRouters][_wPWNumChar]   = {"Qazqaz11", "Qazqaz11", "Qazqaz11"};
bool          _bWiFiConnected;

void SetupWiFi(){
  uint32_t    ulWiFiWaitMsec      = 10 * lMsecPerSec; //mSec to wait for connect
  uint32_t    ulWiFiTimeoutMsec;

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  _bWiFiConnected= false;
#if 0
  for (int wRouterNum= 0; !_bWiFiConnected && (wRouterNum < _wNumRouters); wRouterNum++){
    Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< _acRouterNames[wRouterNum] << "," << _acRouterPWs[wRouterNum] << ")" << endl;
    WiFi.begin(_acRouterNames[wRouterNum], _acRouterPWs[wRouterNum]);
    //Wait for WL_CONNECTED
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
  } //for(int wRouterNum=0;...
#endif  //0
  for (int wRouterNum= 0; !_bWiFiConnected && (wRouterNum < _wNumRouters); wRouterNum++){
    oWiFiMulti.addAP(_acRouterNames[wRouterNum], _acRouterPWs[wRouterNum]);   //Add Wi-Fi networks to connect to
  } //for(int wRouterNum=0;...

  //Serial.println("Connecting ...");
  Serial << LOG0 << "SetupWiFi(): Call oWiFiMulti.run() ";
  while (oWiFiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    //Serial.print('.');
    Serial << ".";
  }
  //Serial.println('\n');
  Serial << endl;
  _bWiFiConnected= true;

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
