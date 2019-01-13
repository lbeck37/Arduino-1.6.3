//BeckE8266WiFiLib.cpp, 1/12/19
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
ESP8266WebServer     *_pSoftAPWebServer;
IPAddress             _oStationIPAddress;
IPAddress             _oAccessPtIPAddress;

bool      bReceivedIPAddress;

//Local function protos
void      handleRoot            ();
void      HandleWiFiCredentials ();
void      handleNotFound        ();
bool      bWiFiBegin             (const char szRouterName[], const char szRouterPW[]);

bool bSetupWiFi(const char szRouterName[], const char szRouterPW[]){
  Serial << LOG0 << "SetupWiFi()(BeckWiFiLib.cpp): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);
  return bWiFiBegin(szRouterName, szRouterPW);
  //return;
} //bSetupWiFi


void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
  case WIFI_EVENT_STAMODE_CONNECTED:          //0
    Serial << LOG0 << "WiFiEvent(): " << event << " Connected to: " << WiFi.SSID() << endl;
    break;
  case WIFI_EVENT_STAMODE_DISCONNECTED:       //1
    Serial << LOG0 << "WiFiEvent(): " << event << " Disconnected" << endl;
    break;
  case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:    //2
    Serial << LOG0 << "WiFiEvent(): " << event << " Authmode change" << endl;
    break;
  case WIFI_EVENT_STAMODE_GOT_IP:             //3
    Serial << LOG0 << "WiFiEvent(): " << event << " Got IP address: " << WiFi.localIP() << endl;
    bReceivedIPAddress= true;
    break;
  case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:       //4
    Serial << LOG0 << "WiFiEvent(): " << event << " DHCP Timeout" << endl;
    break;
  case WIFI_EVENT_SOFTAPMODE_STACONNECTED:    //5
    Serial << LOG0 << "WiFiEvent(): " << event << " AP Station Connected" << endl;
    break;
  case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED: //6
    Serial << LOG0 << "WiFiEvent(): " << event << " AP Station Disconnected" << endl;
    break;
  case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED: //7
    //Logging is turned off because it happens too much when SoftAP is on
    //Serial << LOG0 << "WiFiEvent(): " << event << " Probe Request Received" << endl;
    break;
  case WIFI_EVENT_MAX:                       //8
    Serial << LOG0 << "WiFiEvent(): " << event << " Event Max or Event Any" << endl;
    break;
  case WIFI_EVENT_MODE_CHANGE:               //9
    Serial << LOG0 << "WiFiEvent(): " << event << " Event Mode Change" << endl;
    break;
  default:
    Serial << LOG0 << "WiFiEventHandler(): Executed default switch for event= " << event << endl;
    break;
  } //switch
} //WiFiEvent


bool bWiFiBegin(const char szRouterName[], const char szRouterPW[]){
  bool  bReturn= true;
  Serial << LOG0 << "WiFiBegin(): Connecting to " << szRouterName << " using " << szRouterPW << endl;
  //WiFi.disconnect(true);    // delete old config
  //delay(1000);
  bReceivedIPAddress= false;
  WiFi.onEvent(WiFiEvent);    ////Setup WiFi event handler

  //Start WiFi station
  WiFi.begin(szRouterName, szRouterPW);

  while (!bReceivedIPAddress) { // Wait until we have received IP address from router
    delay(250);
  } //while(!bReceivedIPAddress)
  _oStationIPAddress= WiFi.localIP();
  return bReturn;
} //bWiFiBegin


IPAddress SetupAccessPoint(){
  WiFi.softAP(szAccessPointSSID, szAccessPointPW);             // Start the access point
  _oAccessPtIPAddress= WiFi.softAPIP();
  Serial << LOG0 << "SetupAccessPoint(): " << szAccessPointSSID << " started at " << _oAccessPtIPAddress << endl;
  return(WiFi.softAPIP());
} //SetupAccessPoint


void HandleSoftAPClient(){
  _pSoftAPWebServer->handleClient();    //Listen for HTTP requests from clients
  return;
} //HandleSoftAPClient


void SetupmDNS(IPAddress oIPAddress, char* szName){
  if (MDNS.begin(szName, oIPAddress)) {              // Start the mDNS responder for esp8266.local
    Serial << LOG0 << "SetupmDNS(): mDNS responder started for " << szName << " at " << oIPAddress << endl;
  }
  else {
    Serial << LOG0 << "SetupmDNS(): Error setting up MDNS responder for " << szName << " at " << oIPAddress << endl;
  }
  return;
} //SetupmDNS


void SetupWebServer(IPAddress oIPAddress){
  _pSoftAPWebServer= new ESP8266WebServer(oIPAddress, wWebServerPort);

  _pSoftAPWebServer->on("/", HTTP_GET, handleRoot);         //Function to call when a client requests URI "/"
  _pSoftAPWebServer->on("/WiFiSubmit", HTTP_POST, HandleWiFiCredentials);  //Function to call when a POST request is made to URI "/LED"
  _pSoftAPWebServer->onNotFound(handleNotFound);            //When a client requests an unknown URI
  _pSoftAPWebServer->begin();                               //Actually start the server

  Serial << LOG0 << "SetupWebServer(): HTTP server started at " << oIPAddress << endl;
  return;
} //SetupWebServer


void handleRoot() {
  //When URI / is requested, send a web page with fields for user name and hidden password
  _pSoftAPWebServer->send(200, "text/html",
      "<form action=\"/WiFiSubmit\" method=\"POST\"><input type=\"text\" name=\"WiFi_SSID\" placeholder=\"WiFi SSID\"></br><input type=\"text\" name=\"WiFiPassword\" placeholder=\"WiFi Password\"></br><input type=\"submit\" value=\"Submit\"></form>");
  return;
} //handleRoot


void HandleWiFiCredentials() {
  //If a POST request is made to URI /WiFiSubmit
  Serial << LOG0 << "HandleWiFiCredentials(): Received |" << _pSoftAPWebServer->arg("WiFi_SSID") << "|" <<
      _pSoftAPWebServer->arg("WiFiPassword") << "| as WiFi |name|password|" << endl;

  _pSoftAPWebServer->send(200, "text/html", "<h1>" + _pSoftAPWebServer->arg("WiFi_SSID") +
      "/" + _pSoftAPWebServer->arg("WiFiPassword") + "/ will be used as WiFi name/password/");
  return;
} //HandleWiFiCredentials


void handleNotFound(){
  _pSoftAPWebServer->send(404, "text/plain", "404: Not found");
  return;
} //handleNotFound
//Last line.
