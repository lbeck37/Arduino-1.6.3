//BeckWiFiLib.cpp, Beck Dec 10, 2018
#include <BeckWiFiLib.h>

void SetupWiFi(){
  Serial << LOG0 << "SetupWiFi(): Call WiFi.mode(WIFI_AP_STA)" << endl;
  WiFi.mode(WIFI_AP_STA);

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

  //wl_status_t eWiFiStatus= WiFi.waitForConnectResult();
  Serial << LOG0 << "SetupWiFi(): Call WiFi.waitForConnectResult()" << endl;
  wl_status_t eWiFiStatus= (wl_status_t)WiFi.waitForConnectResult();

  if(eWiFiStatus == WL_CONNECTED) {
    Serial << LOG0 << "SetupWiFi(): WiFi.waitForConnectResult() returned " << szWiFiStatus(eWiFiStatus) << endl;
    //Serial.printf("\nHTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);
    Serial << LOG0 << "SetupWiFi(): IP address= " << WiFi.localIP() << endl;
/*
    SetupOTAServer(acHostname);
    SetupNTP();
*/
  } //if(eWiFiStatus==WL_CONNECTED)
  else {
    //Serial << LOG0 << " SetupServer(): ERROR: WiFi.waitForConnectResult() returned " << ucWiFiStatus << endl;
    Serial << LOG0 << "SetupWiFi(): ERROR: WiFi.waitForConnectResult() returned " << szWiFiStatus(eWiFiStatus) << endl;
  } //if(eWiFiStatus==WL_CONNECTED)else

  return;
} //SetupWiFi


void SetupBlynkWiFi(void){
  switch (sProjectType){
    case sDevLocal:
      Serial << LOG0 << "SetupWiFi(): Call Blynk.config(" << acBlynkAuthToken << ", IPAddress(192,168,15,191))" << endl;
      Blynk.config(acBlynkAuthToken, IPAddress(192,168,15,191));
      break;
    default:
      Serial << LOG0 << "SetupWiFi(): Call Blynk.config(" << acBlynkAuthToken << ")" << endl;
      Blynk.config(acBlynkAuthToken);
      break;
  } //switch
  Serial << LOG0 << "SetupWiFi(): Blynk.config() returned" << endl;
	return;
}	//SetupBlynkWiFi
//Last line.
