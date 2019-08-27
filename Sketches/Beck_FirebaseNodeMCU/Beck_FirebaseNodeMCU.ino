const char szSketchName[]  = "Beck_FirebaseNodeMCU.ino";
const char szFileDate[]    = "8/27/19c";

#include <Arduino.h>
//#include <DHT.h>
//#include <DHT_U.h>
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

// comment this line if you want to use DHT11
//#define TEST

//#define DHTTYPE DHT11
//#define DHTPIN D2

//DHT dht(DHTPIN, DHTTYPE);

void connectToWiFi() {
    delay(10);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(SSID);
    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
    would try to act as both a client and an access-point and could cause
    network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    //start connecting to WiFi
    WiFi.begin(SSID, PASSWORD);
    //while client is not connected to WiFi keep loading
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected to ");
    Serial.println(SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
    connectToWiFi();
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    //dht.begin();
}

void loop() {
    static int  wCount= 0;
    // === Push temperature value to Firebase ===
    wCount++;
    //String tempValueID = Firebase.pushInt("dht11/temperature", random(0, 80));
    String tempValueID = Firebase.pushInt("dht11/temperature", wCount);
    Serial.print("[INFO] temperature: ");
    //Serial.println(random(0, 80));
    Serial.println(wCount);
    if (Firebase.failed()) {
        Serial.print("[ERROR] pushing /dht11/temperature failed:");
        Serial.println(Firebase.error());
        return;
    }
    Serial.print("[INFO] pushed: /dht11/temperature \tkey: ");
    Serial.println(tempValueID);

    // === Push humidity value to Firebase ===
    wCount++;
    //String humValueID = Firebase.pushInt("dht11/humidity", random(0, 80));
    String humValueID = Firebase.pushInt("dht11/humidity", wCount);
    Serial.print("[INFO] humidity: ");
    //Serial.println(random(0, 80));
    Serial.println(wCount);
    if (Firebase.failed()) {
        Serial.print("[ERROR] pushing /dht11/humidity failed:");
        Serial.println(Firebase.error());
        return;
    }
    Serial.print("[INFO] pushed: /dht11/humidity    \tkey: ");
    Serial.println(humValueID);
    Serial.println();
    delay(1000);
/*
        // === Read and Log temperature and humidity to Serial Monitor ===
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        Serial.print("[INFO] Current Humidity = ");
        Serial.print(h);
        Serial.println(" %");
        Serial.print("[INFO] Current Temperature = ");
        Serial.print(t);
        Serial.println(" °C");
        
        // Eliminate abnormal values
        if ((t >= -15 && t <= 80) && (h >= 0 && h <= 100)) {
            // === Push temperature value to Firebase ===
            String tempValueID = Firebase.pushFloat("dht11/temperature", t);
            if (Firebase.failed()) {
                Serial.print("[ERROR] pushing /dht11/temperature failed:");
                Serial.println(Firebase.error());
                return;
            }
            Serial.print("[INFO] pushed: /dht11/temperature \tkey: ");
            Serial.println(tempValueID);

            // === Push humidity value to Firebase ===
            String humValueID = Firebase.pushFloat("dht11/humidity", h);
            if (Firebase.failed()) {
                Serial.print("[ERROR] pushing /dht11/humidity failed:");
                Serial.println(Firebase.error());
                return;
            }
            Serial.print("[INFO] pushed: /dht11/humidity    \tkey: ");
            Serial.println(humValueID);
            Serial.println();
        } else {
            Serial.println("[ERROR] Wrong values!");
        }
    delay(1000);
*/

}
