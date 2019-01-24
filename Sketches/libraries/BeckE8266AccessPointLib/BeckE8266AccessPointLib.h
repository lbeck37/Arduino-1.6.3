//BeckE8266AccessPointLib.h, 1/13/19
#include <ESP8266WebServer.h>

extern IPAddress             _oAccessPtIPAddress;
extern ESP8266WebServer     *_pSoftAPWebServer;

IPAddress SetupAccessPoint    ();
void      HandleSoftAPClient  ();
void      SetupWebServer      (IPAddress oIPAddress);
//Last line.
