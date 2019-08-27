const char szSketchName[]  = "Beck_FirebaseNodeMCU.ino";
const char szFileDate[]    = "8/27/19k";

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include <Streaming.h>
//#include "Creds.h"

/*
#define SSID "PLANET_D9CM"
#define PASSWORD "FVMSJ4EZ"

#define FIREBASE_HOST "medium-iot-project.firebaseio.com"
#define FIREBASE_AUTH "lr6Wp38WJNod6dOmVsbBPstVfPzf8QpObXf8fJIv"
*/
#define SSID "Aspot24"
#define PASSWORD "Qazqaz11"

#define FIREBASE_HOST "simpleiot-65405.web.app"
#define FIREBASE_AUTH "AIzaSyDTXPBe2Bgy003pA8RgOTPVIURZ8EEnG3Y"


void connectToWiFi() {
    delay(10);
    Serial << "connectToWiFi(): Connecting to " << SSID << endl;
    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
    would try to act as both a client and an access-point and could cause
    network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial << endl << "connectToWiFi(): WiFi connected to " << SSID << "IP address: " << WiFi.localIP() << endl << endl;
} //connectToWiFi


void setup() {
    Serial.begin(115200);
    delay(100);
    Serial << endl << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
    connectToWiFi();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    return;
} //setup


void loop() {
    static int  wCount= 0;
    // === Push dummy temperature value to Firebase ===
    wCount++;
    String tempValueID = Firebase.pushInt("dht11/temperature", wCount);
    Serial << "loop(): Temperature= " << wCount << endl;
    if (Firebase.failed()) {
      Serial << "loop(): ERROR pushing /dht11/temperature failed, error= " << Firebase.error() << endl;
    } //if(Firebase.failed())
    else {
      Serial << "loop(): Pushed: /dht11/temperature, key= " << tempValueID << endl;
    } //if(Firebase.failed()) else

    // === Push dummy humidity value to Firebase ===
    wCount++;
    String humValueID = Firebase.pushInt("dht11/humidity", wCount);
    Serial << "loop(): Humidity= " << wCount << endl;
    if (Firebase.failed()) {
      Serial << "loop(): ERROR pushing /dht11/humidity failed, error= " << Firebase.error() << endl;
    } //if(Firebase.failed())
    else {
      Serial << "loop(): Pushed: /dht11/humidity, key= " << humValueID << endl;
    } //if(Firebase.failed()) else

    delay(5000);
    return;
} //loop
//Last line
