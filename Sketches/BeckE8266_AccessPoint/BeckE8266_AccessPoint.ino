const char szSketchName[]  = "BeckE8266_AccessPoint.ino";
const char szFileDate[]    = "Lenny 1/14/19t";

#include <BeckMiniLib.h>
#include <BeckE8266WiFiLib.h>
#include <BeckE8266AccessPointLib.h>
#include <BeckE8266NTPLib.h>

#define DO_ACCESS_PT    false

static const char     szRouterName[]        = "Aspot24";
static const char     szRouterPW[]          = "Qazqaz11";
static const int      _wLedPin              = 2;
char                  _szDNSName[]          = "beckdev1";
unsigned long         _ulNextHandlerMsec    = 0;
unsigned long         _ulHandlerSpacing     = 10 * 1000;
int                   _wCount               = 1;

void setup(void){
  Serial.begin(115200);
  delay(10);
  Serial << endl << LOG0 <<"setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  pinMode(_wLedPin, OUTPUT);
  bSetupWiFi(szRouterName, szRouterPW);
  SetupNTP();
#if DO_ACCESS_PT
  SetupAccessPoint();
  SetupWebServer(_oAccessPtIPAddress);
#endif  //DO_ACCESS_PT
  //SetupmDNS(_oStationIPAddress, _szDNSName);
  return;
} //setup


void loop(void){
#if DO_ACCESS_PT
  HandleSoftAPClient();       //Listen for HTTP requests from clients
#endif  //DO_ACCESS_PT
  if (millis() >= _ulNextHandlerMsec){
    _ulNextHandlerMsec= millis() + _ulHandlerSpacing;
    Serial << LOG0 <<"loop(): _wCount= " << _wCount++ << endl;
  } //f (millis()>=_ulNextHandlerMsec)
  return;
} //loop

//Last line.
