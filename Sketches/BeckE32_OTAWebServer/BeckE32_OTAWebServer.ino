// BeckE32_OTAWebServer.ino
#include <BeckE32_OTAWebServerLib.h>

const char* szOTAServerHostName = "BeckOTA_012119a";

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial.printf("\nBeckE32_OTAWebServer.ino 1/22/19b\nsetup(): Begin\n");

  Serial.printf("\nsetup(): Call SetupOTAServer()\n");
  SetupOTAServer(szOTAServerHostName);
  Serial.printf("\nsetup(): SetupOTAServer() returned\n");
  return;
} //setup


void loop()
{
  //HandleOTAServer();
  return;
} //loop
//Last line.
