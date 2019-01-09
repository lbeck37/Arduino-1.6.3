const char szSketchName[]  = "BeckE8266_GuideWebServerPOST.ino";
const char szFileDate[]    = "Lenny 1/8/19n";

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Streaming.h>

ESP8266WebServer      *pWiFiConfigServer;

static const int      led                   = 2;
static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";
static const char     szDNSName[]           = "beckdev1";

static const char     szAccessPointSSID[]   = "BeckESP8266AccessPoint";
static const char     szAccessPointPW[]     = "Qazqaz11";

void handleRoot();
void HandleWiFiCredentials();
void handleNotFound();

//Not sure why only these functions need prototypes
void SetupmDNS();
void SetupWebServer();


void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  Serial << "setup(): Connecting to " << szRouterName << " using " << szRouterPW << endl;

  pinMode(led, OUTPUT);

  WiFi.begin(szRouterName, szRouterPW);             // Connect to the network
  Serial << "setup(): Connecting .";

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(250);
    Serial << "." ;
  }
  Serial << endl << "setup(): Connected to " << WiFi.SSID() << endl;
  Serial << "setup(): IP address: " << WiFi.localIP() << endl;

  SetupmDNS();
  SetupWebServer();
  SetupAccessPoint();
  return;
} //setup


void SetupmDNS(){
  Serial << "SetupmDNS(): Begin" << endl;
  if (MDNS.begin(szDNSName)) {              // Start the mDNS responder for esp8266.local
    Serial << "SetupmDNS(): mDNS responder started for " << szDNSName << endl;
  }
  else {
    Serial << "SetupmDNS(): Error setting up MDNS responder for " << szDNSName << endl;
  }
  return;
} //SetupmDNS


void SetupWebServer(){
  Serial << "SetupWebServer(): Construct Web Server" << endl;
  pWiFiConfigServer= new ESP8266WebServer(80);

  pWiFiConfigServer->on("/", HTTP_GET, handleRoot);         //Function to call when a client requests URI "/"
  pWiFiConfigServer->on("/WiFiSubmit", HTTP_POST, HandleWiFiCredentials);  //Function to call when a POST request is made to URI "/LED"
  pWiFiConfigServer->onNotFound(handleNotFound);            //When a client requests an unknown URI
  pWiFiConfigServer->begin();                               //Actually start the server

  Serial << "SetupWebServer(): HTTP server started" << endl;
  return;
} //SetupWebServer


void SetupAccessPoint(){
  Serial << "SetupAccessPoint(): Begin" << endl;
  WiFi.softAP(szAccessPointSSID, szAccessPointPW);             // Start the access point
  Serial << "SetupAccessPoint(): Access Point " << szAccessPointSSID << " started" << endl;
  Serial << "SetupAccessPoint(): IP address: " << WiFi.softAPIP() << endl;
  return;
} //SetupAccessPoint


void loop(void){
  pWiFiConfigServer->handleClient();    //Listen for HTTP requests from clients
  return;
} //loop


void handleRoot() {
  //When URI / is requested, send a web page with fields for user name and hidden password
  pWiFiConfigServer->send(200, "text/html",
      "<form action=\"/WiFiSubmit\" method=\"POST\"><input type=\"text\" name=\"WiFi_SSID\" placeholder=\"WiFi SSID\"></br><input type=\"text\" name=\"WiFiPassword\" placeholder=\"WiFi Password\"></br><input type=\"submit\" value=\"Submit\"></form>");
  return;
} //handleRoot


void HandleWiFiCredentials() {
  //If a POST request is made to URI /WiFiSubmit
  Serial << "HandleWiFiCredentials(): Begin" << endl;
  Serial << "HandleWiFiCredentials(): WiFi SSID= " << pWiFiConfigServer->arg("WiFi_SSID") << endl;
  Serial << "HandleWiFiCredentials(): Password = " << pWiFiConfigServer->arg("WiFiPassword") << endl;
  pWiFiConfigServer->send(200, "text/html", "<h1>Received " + pWiFiConfigServer->arg("WiFi_SSID") + " for WiFi SSID");
  return;
} //HandleWiFiCredentials


void handleNotFound(){
  pWiFiConfigServer->send(404, "text/plain", "404: Not found");
  return;
} //handleNotFound
//Last line.
