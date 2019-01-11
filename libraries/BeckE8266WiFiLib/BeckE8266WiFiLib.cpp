//BeckE8266WiFiLib.cpp, 1/9/19

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

//Function prototypes
void      handleRoot            ();
void      HandleWiFiCredentials ();
void      handleNotFound        ();

//Function protos
void SetupWiFiHandlers();

void SetupWiFi(const char szRouterName[], const char szRouterPW[]){
  //IPAddress oIPAddress;

  Serial << LOG0 << "SetupWiFi()(BeckWiFiLib.cpp): Call WiFi.mode(WIFI_STA)" << endl;
  WiFi.mode(WIFI_STA);

  Serial << LOG0 << "SetupWiFi(): Call WiFi.begin("<< szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  //oIPAddress= WiFi.localIP();
  //Serial << LOG0 << "SetupWiFi(): oIPAddress: " << oIPAddress << endl;
  SetupWiFiHandlers();
  return;
} //SetupWiFi


void SetupWiFiHandlers(){
  Serial << LOG0 << "SetupWiFiHandlers(): Setup WiFi.onEvent" << endl;
  WiFi.onEvent([](WiFiEvent_t oEvent) {
    Serial.printf("Event wifi -----> %d\n", oEvent);
  });
  return;
}  //SetupWiFiHandlers


/*
IPAddress SetupWiFi(char szRouterName[], char szRouterPW[]){
  Serial << "SetupWiFi(): Connecting to " << szRouterName << " using " << szRouterPW << endl;
  WiFi.begin(szRouterName, szRouterPW);             // Connect to the network
  Serial << "SetupWiFi(): Connecting .";

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(250);
    Serial << "." ;
  }
  Serial << endl << "SetupWiFi(): Connected to " << WiFi.SSID() <<" at " << WiFi.localIP() << endl;
  return(WiFi.localIP());
} //SetupWiFi
*/


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
