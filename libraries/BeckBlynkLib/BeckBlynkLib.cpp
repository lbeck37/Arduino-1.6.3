//BeckBlynkLib.cpp, Beck Dec 10, 2018
#include <BeckBlynkLib.h>

void SetupBlynkWiFi(void){
  switch (sProjectType){
    case sDevLocal:
      Serial << LOG0 << "SetupWiFi(): Call Blynk.config(" << acBlynkAuthToken << ", IPAddress(192,168,15,191))" << endl;
      Blynk.config(acBlynkAuthToken, IPAddress(192,168,15,191));
      break;
    default:
      Serial << LOG0 << "SetupWiFi(): Call Blynk.config(" << acBlynkAuthToken << ")" << endl;
      Blynk.config(acBlynkAuthToken);
      break;
  } //switch
  Serial << LOG0 << "SetupWiFi(): Blynk.config() returned" << endl;
	return;
}	//SetupBlynkWiFi
//Last line.
