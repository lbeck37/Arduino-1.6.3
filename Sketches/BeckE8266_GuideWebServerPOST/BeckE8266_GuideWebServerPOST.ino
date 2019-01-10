const char szSketchName[]  = "BeckE8266_GuideWebServerPOST.ino";
const char szFileDate[]    = "Lenny 1/9/19r";

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Streaming.h>

ESP8266WebServer      *pWiFiConfigServer;

static const int      led                   = 2;
static const int      wWebServerPort        = 80;
static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";
static const char     szDNSName[]           = "beckdev1";
static const char     szAccessPointSSID[]   = "BeckESP8266AccessPoint";
static const char     szAccessPointPW[]     = "Qazqaz11";

IPAddress             _oStationIPAddress;
IPAddress             _oAccessPtIPAddress;

void handleRoot();
void HandleWiFiCredentials();
void handleNotFound();

//Not sure why only these functions need prototypes
IPAddress SetupWiFi();
IPAddress SetupAccessPoint();
void      SetupmDNS();
void      SetupWebServer(IPAddress oIPAddress);

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  pinMode(led, OUTPUT);
  _oStationIPAddress= SetupWiFi();
  SetupmDNS();
  _oAccessPtIPAddress= SetupAccessPoint();
  SetupWebServer(_oAccessPtIPAddress);
  return;
} //setup


void loop(void){
  pWiFiConfigServer->handleClient();    //Listen for HTTP requests from clients
  return;
} //loop


IPAddress SetupWiFi(){
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


IPAddress SetupAccessPoint(){
  WiFi.softAP(szAccessPointSSID, szAccessPointPW);             // Start the access point
  _oAccessPtIPAddress= WiFi.softAPIP();
  Serial << "SetupAccessPoint(): " << szAccessPointSSID << " started at " << _oAccessPtIPAddress << endl;
  return(WiFi.softAPIP());
} //SetupAccessPoint


void SetupmDNS(){
  if (MDNS.begin(szDNSName)) {              // Start the mDNS responder for esp8266.local
    Serial << "SetupmDNS(): mDNS responder started for " << szDNSName << endl;
  }
  else {
    Serial << "SetupmDNS(): Error setting up MDNS responder for " << szDNSName << endl;
  }
  return;
} //SetupmDNS


void SetupWebServer(IPAddress oIPAddress){
  pWiFiConfigServer= new ESP8266WebServer(oIPAddress, wWebServerPort);

  pWiFiConfigServer->on("/", HTTP_GET, handleRoot);         //Function to call when a client requests URI "/"
  pWiFiConfigServer->on("/WiFiSubmit", HTTP_POST, HandleWiFiCredentials);  //Function to call when a POST request is made to URI "/LED"
  pWiFiConfigServer->onNotFound(handleNotFound);            //When a client requests an unknown URI
  pWiFiConfigServer->begin();                               //Actually start the server

  Serial << "SetupWebServer(): HTTP server started at " << oIPAddress << endl;
  return;
} //SetupWebServer


void handleRoot() {
  //When URI / is requested, send a web page with fields for user name and hidden password
  pWiFiConfigServer->send(200, "text/html",
      "<form action=\"/WiFiSubmit\" method=\"POST\"><input type=\"text\" name=\"WiFi_SSID\" placeholder=\"WiFi SSID\"></br><input type=\"text\" name=\"WiFiPassword\" placeholder=\"WiFi Password\"></br><input type=\"submit\" value=\"Submit\"></form>");
  return;
} //handleRoot


void HandleWiFiCredentials() {
  //If a POST request is made to URI /WiFiSubmit
  Serial << "HandleWiFiCredentials(): Received " << pWiFiConfigServer->arg("WiFi_SSID") << "/" <<
      pWiFiConfigServer->arg("WiFiPassword") << " as WiFi name/password" << endl;

  pWiFiConfigServer->send(200, "text/html", "<h1>" + pWiFiConfigServer->arg("WiFi_SSID") +
      "/" + pWiFiConfigServer->arg("WiFiPassword") + " will be used as WiFi name/password");
  return;
} //HandleWiFiCredentials


void handleNotFound(){
  pWiFiConfigServer->send(404, "text/plain", "404: Not found");
  return;
} //handleNotFound
//Last line.
