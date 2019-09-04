const char szSketchName[]  = "Beck_FirebaseNodeMCU.ino";
const char szFileDate[]    = "9/4/19a";

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

//Fingerprints set in FirebaseHttpClient.h:
  // firebaseio.com:  "B6:F5:80:C8:B1:DA:61:C1:07:9D:80:42:D8:A9:1F:AF:9F:C8:96:7D"
  //  web.app:        "7D:F4:C4:BB:81:2F:EE:72:29:BD:36:42:6D:80:A7:A7:B3:F9:28:B9"

//#define FIREBASE_HOST "https://fir-mariestep1.firebaseio.com/"
//#define FIREBASE_HOST "fir-mariestep1.firebaseio.com/"

//#define FIREBASE_HOST "simpleiot-65405.firebaseio.com"
/*
#define FIREBASE_HOST "//simpleiot-65405.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyDTXPBe2Bgy003pA8RgOTPVIURZ8EEnG3Y"
*/
#define FIREBASE_HOST "//thermo-2b830.firebaseio.com/"
//#define FIREBASE_HOST "//thermo-2b830.web.app/"
#define FIREBASE_AUTH "AIzaSyAkFumb-wjDUQ9HQjTOoHeXqTKztFSqf6o"

//#define FIREBASE_HOST "//thermo-2b830.web.app/"    //
//#define FIREBASE_AUTH "AIzaSyAkFumb-wjDUQ9HQjTOoHeXqTKztFSqf6o"

/*
#define FIREBASE_HOST "//fir-mariestep1.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyBVOUnaaCjuoFWLsiyVi7ZSW_PsiHLam1A"
*/


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
    //Serial << "setup(): Call Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH)" << endl;
    Serial << "setup(): Call Firebase.begin(" << FIREBASE_HOST << ", " << FIREBASE_AUTH << ")" << endl;
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   //Does not set error
    return;
} //setup


void loop() {
    static float  fCount= 20.0;
    // === Push dummy temperature value to Firebase ===
    fCount += 0.01;
    Serial << endl << "loop(): Call Firebase.setFloat(DegF, " << fCount << ")" << endl;
    Firebase.setFloat("DegF", fCount);

    if (Firebase.failed()) {
      //Serial.print("loopp():Error setting DegF, error= ");
      //Serial.println(Firebase.error());
      Serial << "loop():Error setting DegF, error= " << Firebase.error() << endl;
    } //if(Firebase.failed())
    else {
      Serial << "loop(): Set DegF to " << fCount << " in Firebase" << endl;
    } //if(Firebase.failed()) else

    // === Get Setpoint from Firebase ===
    Serial << "loop(): Call Firebase.getFloat(Setpoint)" << ")" << endl;
    float fNewSetpoint= Firebase.getFloat("Setpoint");

    if (Firebase.success()) {
      Serial << "loop(): fNewSetpoint= " << fNewSetpoint << endl;
    } //if(Firebase.failed())
    else {
      //Serial << "loop(): Set DegF to " << fCount << " in Firebase" << endl;
      Serial << "loop(): Error getting fNewSetpoint, error= " << Firebase.error() << endl;
    } //if(Firebase.failed()) else

    delay(5000);
    return;
} //loop
//Last line
