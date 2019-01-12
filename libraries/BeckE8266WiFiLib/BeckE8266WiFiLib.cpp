//BeckE8266WiFiLib.cpp, 1/11/19
#include <BeckE8266WiFiLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <Streaming.h>
#include <BeckMiniLib.h>

const int      wWebServerPort        = 80;
const char     szAccessPointSSID[]   = "BeckESP8266AccessPoint";
const char     szAccessPointPW[]     = "Qazqaz11";

//The following are declared external in BeckE8266WiFiLib.
ESP8266WebServer      *pConfigWiFiServer;
IPAddress             _oStationIPAddress;
IPAddress             _oAccessPtIPAddress;

//Local function protos
void      handleRoot            ();
void      HandleWiFiCredentials ();
void      handleNotFound        ();
void      WiFiBegin             (const char szRouterName[], const char szRouterPW[]);

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
  Serial << LOG0 << "SetupWiFi()(BeckWiFiLib.cpp): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);
  WiFiBegin(szRouterName, szRouterPW);
  return;
} //SetupWiFi


void WiFiEvent(WiFiEvent_t event) {
  Serial << LOG0 << "WiFiEvent(): Received event: " << event << endl;
  switch(event) {
  case WIFI_EVENT_STAMODE_CONNECTED:          //0
    //Serial << LOG0 << "WiFiEvent(): Connected" << endl;
    Serial << LOG0 << "WiFiEvent(): Connected to: " << WiFi.SSID() << endl;
    break;
  case WIFI_EVENT_STAMODE_DISCONNECTED:       //1
    Serial << LOG0 << "WiFiEvent(): Disconnected" << endl;
    break;
  case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:    //2
    Serial << LOG0 << "WiFiEvent(): Authmode change" << endl;
    break;
  case WIFI_EVENT_STAMODE_GOT_IP:             //3
    Serial << LOG0 << "WiFiEvent(): Got IP address: " << WiFi.localIP() << endl;
    break;
  case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:       //4
    Serial << LOG0 << "WiFiEvent(): DHCP Timeout" << endl;
    break;
  case WIFI_EVENT_SOFTAPMODE_STACONNECTED:    //5
    Serial << LOG0 << "WiFiEvent(): AP Station Connected" << endl;
    break;
  case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED: //6
    Serial << LOG0 << "WiFiEvent(): AP Station Disconnected" << endl;
    break;
  case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED: //7
    Serial << LOG0 << "WiFiEvent(): Probe Request Received" << endl;
    break;
  case WIFI_EVENT_MAX:                       //8
    Serial << LOG0 << "WiFiEvent(): Event Max or Event Any" << endl;
    break;
  case WIFI_EVENT_MODE_CHANGE:               //9
    Serial << LOG0 << "WiFiEvent(): Event Mode Change" << endl;
    break;
  default:
    Serial << LOG0 << "WiFiEventHandler(): Executed default switch for event= " << event << endl;
    break;
  } //switch
} //WiFiEvent


void WiFiBegin(const char szRouterName[], const char szRouterPW[]){
  Serial << LOG0 << "WiFiBegin(): Connecting to " << szRouterName << " using " << szRouterPW << endl;
  WiFi.disconnect(true);    // delete old config
  delay(1000);
  WiFi.onEvent(WiFiEvent);    ////Setup WiFi event handler

  //Start WiFi station
  WiFi.begin(szRouterName, szRouterPW);

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(250);
  } //while
  return;
} //WiFiBegin


IPAddress SetupAccessPoint(){
  WiFi.softAP(szAccessPointSSID, szAccessPointPW);             // Start the access point
  _oAccessPtIPAddress= WiFi.softAPIP();
  Serial << "SetupAccessPoint(): " << szAccessPointSSID << " started at " << _oAccessPtIPAddress << endl;
  return(WiFi.softAPIP());
} //SetupAccessPoint


void SetupmDNS(IPAddress oIPAddress, char* szName){
  if (MDNS.begin(szName, oIPAddress)) {              // Start the mDNS responder for esp8266.local
    Serial << "SetupmDNS(): mDNS responder started for " << szName << " at " << oIPAddress << endl;
  }
  else {
    Serial << "SetupmDNS(): Error setting up MDNS responder for " << szName << " at " << oIPAddress << endl;
  }
  return;
} //SetupmDNS


void SetupWebServer(IPAddress oIPAddress){
  pConfigWiFiServer= new ESP8266WebServer(oIPAddress, wWebServerPort);

  pConfigWiFiServer->on("/", HTTP_GET, handleRoot);         //Function to call when a client requests URI "/"
  pConfigWiFiServer->on("/WiFiSubmit", HTTP_POST, HandleWiFiCredentials);  //Function to call when a POST request is made to URI "/LED"
  pConfigWiFiServer->onNotFound(handleNotFound);            //When a client requests an unknown URI
  pConfigWiFiServer->begin();                               //Actually start the server

  Serial << "SetupWebServer(): HTTP server started at " << oIPAddress << endl;
  return;
} //SetupWebServer


void handleRoot() {
  //When URI / is requested, send a web page with fields for user name and hidden password
  pConfigWiFiServer->send(200, "text/html",
      "<form action=\"/WiFiSubmit\" method=\"POST\"><input type=\"text\" name=\"WiFi_SSID\" placeholder=\"WiFi SSID\"></br><input type=\"text\" name=\"WiFiPassword\" placeholder=\"WiFi Password\"></br><input type=\"submit\" value=\"Submit\"></form>");
  return;
} //handleRoot


void HandleWiFiCredentials() {
  //If a POST request is made to URI /WiFiSubmit
  Serial << "HandleWiFiCredentials(): Received " << pConfigWiFiServer->arg("WiFi_SSID") << "/" <<
      pConfigWiFiServer->arg("WiFiPassword") << " as WiFi name/password" << endl;

  pConfigWiFiServer->send(200, "text/html", "<h1>" + pConfigWiFiServer->arg("WiFi_SSID") +
      "/" + pConfigWiFiServer->arg("WiFiPassword") + " will be used as WiFi name/password");
  return;
} //HandleWiFiCredentials


void handleNotFound(){
  pConfigWiFiServer->send(404, "text/plain", "404: Not found");
  return;
} //handleNotFound
//Last line.
