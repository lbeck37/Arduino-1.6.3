//Beck_ElectronicClinic_Firebase.ino, 12/5/19f
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

// Set these to run example.
/*
#define FIREBASE_HOST "fahad-ba398.firebaseio.com"
#define FIREBASE_AUTH "d0vy3ZgNMiwszDZsZ3iT1jg9enRLbKypvadDEPz1"
#define WIFI_SSID "ZONG MBB-E8231-6E63"
#define WIFI_PASSWORD "08659650"
*/

//#define FIREBASE_HOST "fahad-ba398.firebaseio.com"
//#define FIREBASE_AUTH "d0vy3ZgNMiwszDZsZ3iT1jg9enRLbKypvadDEPz1"
//#define WIFI_SSID "ZONG MBB-E8231-6E63"
//#define WIFI_PASSWORD "08659650"

#define FIREBASE_HOST "//thermo-2b830.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyAkFumb-wjDUQ9HQjTOoHeXqTKztFSqf6o"
#define WIFI_SSID "Aspot24"
#define WIFI_PASSWORD "Qazqaz11"

String myString;
int vr = A0; // variable resistor connected
int sdata = 0; // The variable resistor value will be stored in sdata.

void setup(){
  // Debug console
  Serial.begin(115200);
  pinMode(vr ,INPUT);

  // connect to wifi.
  pinMode(D0,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  } //while
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setString("Variable/Value","fahad");
  if (!Firebase.success()){
    Serial.print(Firebase.error());
  } //if(!Firebase.success())
} //setup


void loop(){
  sdata = analogRead(vr);
  myString = String(sdata);
  Serial.println(myString);
  Firebase.setString("Variable/Value",myString);
  if (!Firebase.success()){
    Serial.print(Firebase.error());
  } //if(!Firebase.success())
delay(5000);
} //loop
//Last line.
