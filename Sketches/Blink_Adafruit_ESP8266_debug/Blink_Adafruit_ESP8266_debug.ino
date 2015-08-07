#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

void setup() {
	pinMode(0, OUTPUT);
}	//setup

void loop() {
	digitalWrite(0, HIGH);
  delay(1500);
  // delay(500);
	digitalWrite(0, LOW);
	delay(500);
}	//loop
