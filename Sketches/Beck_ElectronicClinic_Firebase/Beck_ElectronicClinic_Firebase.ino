//Beck_ElectronicClinic_Firebase.ino, 12/5/19j
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Streaming.h>

#define FIREBASE_HOST "//thermo-2b830.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyAkFumb-wjDUQ9HQjTOoHeXqTKztFSqf6o"
#define WIFI_SSID "Aspot24"
#define WIFI_PASSWORD "Qazqaz11"

String myString;
//int vr = A0; // variable resistor connected
//int sdata = 0; // The variable resistor value will be stored in sdata.

void setup(){
  // Debug console
  Serial.begin(115200);
  //pinMode(vr ,INPUT);

  // connect to wifi.
  //pinMode(D0,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  Serial << "Connecting to WiFi router" << endl;
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  } //while
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
/*
  Firebase.setString("Variable/Value","fahad");
  if (!Firebase.success()){
    Serial.print(Firebase.error());
  } //if(!Firebase.success())
*/
  return;
} //setup


void loop(){
	//sdata = analogRead(vr);
/*
	myString = String(sdata);
	Serial.println(myString);
	Firebase.setString("Variable/Value",myString);
	if (!Firebase.success()){
		Serial.print(Firebase.error());
	} //if(!Firebase.success())
*/
	static int wSetpoint= 0;
	Serial << "Setting Setpoint to " << ++wSetpoint << endl;
	Serial << "loop(): Call Firebase.setInt()" << endl;
	Firebase.setInt("Setpoint", wSetpoint);

	Serial << "loop(): Firebase.error()= " << Firebase.error() << endl;
/*
	if (Firebase.failed()){
		Serial << "loop(): Previous call failed, Firebase.error()= " << Firebase.error() << endl;
	} //if(!Firebase.success())
*/

	delay(5000);
	return;
} //loop
//Last line.
