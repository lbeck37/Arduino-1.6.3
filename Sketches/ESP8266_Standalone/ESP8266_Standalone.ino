// 09/21/16 Beck, Sparkfun Thing example
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "55bce1afbf894b3bb67b7ea34f29d45a";

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, "Aspot24", "Qazqaz11");
}

void loop()
{
  Blynk.run();
}

