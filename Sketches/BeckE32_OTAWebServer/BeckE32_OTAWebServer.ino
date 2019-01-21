// BeckE32_OTAWebServer.ino
#include <BeckE32_OTAWebServerLib.h>

const char* szOTAServerHostName = "BeckOTA";

void setup()
{
  SetupOTAServer(szOTAServerHostName);
  return;
} //setup


void loop()
{
  HandleOTAServer();
  return;
} //loop
//Last line.
