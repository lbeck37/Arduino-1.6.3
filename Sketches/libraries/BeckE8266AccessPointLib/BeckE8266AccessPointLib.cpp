//BeckE8266AccessPointLib.cpp, 2/3/19
#include <BeckE8266AccessPointLib.h>
#include <BeckLogLib.h>
#include <BeckMiniLib.h>
#include <ESP8266WebServer.h>
//#include <Streaming.h>

const int      wWebServerPort        = 80;

IPAddress             _oAccessPtIPAddress;
ESP8266WebServer     *_pSoftAPWebServer;

//Local function prototypes
void      SetupWebServer        (IPAddress oIPAddress);
void      handleRoot            ();
void      HandleWiFiCredentials ();
void      handleNotFound        ();

void SetupWiFiNameServer(const char *szAccessPointSSID, const char *szAccessPointPW){
  WiFi.softAP(szAccessPointSSID, szAccessPointPW);             // Start the access point
  _oAccessPtIPAddress= WiFi.softAPIP();
  SetupWebServer(_oAccessPtIPAddress);
  Serial << LOG0 << "SetupWiFiNameServer(): Web Server started at " << _oAccessPtIPAddress <<
      " on " << szAccessPointSSID << "/" << szAccessPointPW << endl;
  return;
} //SetupWiFiNameServer


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
