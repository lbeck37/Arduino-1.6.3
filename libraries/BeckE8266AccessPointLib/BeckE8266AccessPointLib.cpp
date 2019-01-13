//BeckE8266AccessPointLib.cpp, 1/13/19
#include <BeckE8266AccessPointLib.h>
#include <Streaming.h>
#include <BeckMiniLib.h>

const int      wWebServerPort        = 80;
const char     szAccessPointSSID[]   = "BeckESP8266AccessPoint";
const char     szAccessPointPW[]     = "Qazqaz11";

//The following are declared external in BeckE8266_AccessPointLib.h
IPAddress             _oAccessPtIPAddress;
ESP8266WebServer     *_pSoftAPWebServer;

//Local function prototypes
void      handleRoot            ();
void      HandleWiFiCredentials ();
void      handleNotFound        ();

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
