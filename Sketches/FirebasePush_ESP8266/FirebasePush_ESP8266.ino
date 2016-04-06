// Copyright 2015 Google Inc.
// FirebasePush_ESP8266 is a sample that push a new timestamp to firebase on each reset
#include <BeckLib.h>
#include <Streaming.h>
#include <Firebase.h>

static const char szSketchName[]  = "FirebasePush_ESP8266.ino";
static const char szFileDate[]    = "Apr 5, 2016c";

//static const long   	lSerialMonitorBaud    = 115200;
static const char   	acRouterName[]        = "Aspot24";
static const char   	acRouterPW[]          = "Qazqaz11";
//static const char   acRouterName[]        = "TrailheadBoise";
//static const char   acRouterPW[]          = "Trailhead2015";
//static const String   acHostname[]          = "esp39";
static const String 	acDatabaseURL         = "intense-fire-3958.firebaseio.com";
static const String 	acFirebaseSecret      = "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
static const String 	acPushPath		      = "/logs";
//static const String acPushJSON		      = "{\".sv\": \"timestamp\"}";
//static const String acPushJSON		      = "{\"Data37\": \"Abcdef37\"}";

String	szLogLine;

void setup() {
  Serial.begin(lSerialMonitorBaud);
  Serial << endl << LOG0 << " setup(): Initialized serial to " << lSerialMonitorBaud << " baud" << endl;

  //Create Firebase client.
  Serial << LOG0 << " setup(): Call Firebase('" << acDatabaseURL << "').auth('" << acFirebaseSecret << "')" << endl;
  Firebase oFBase = Firebase(acDatabaseURL).auth(acFirebaseSecret);

  //Log sketch name and date.
  //Serial << LOG0 << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  szLogLine=  LOG0 + " *setup(): Sketch: " + szSketchName + ", " + szFileDate + ")";
  LogToBoth(oFBase, acPushPath, szLogLine);

  //Connect to wifi.
  //Serial << LOG0 << " setup(): Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
  szLogLine=  LOG0 + " *setup(): Call WiFi.begin(" + acRouterName + ", " + acRouterPW + ")";
  LogToBoth(oFBase, acPushPath, szLogLine);
  WiFi.begin(acRouterName, acRouterPW);

  //Serial.print("connecting");
  //Serial << LOG0 << " setup(): Call WiFi.status(), wait for WL_CONNECTED" << endl;
  szLogLine=  LOG0 + " *setup(): Call WiFi.status(), wait for WL_CONNECTED";
  LogToBoth(oFBase, acPushPath, szLogLine);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //Serial << endl << LOG0 << " setup(): Connected to " << WiFi.localIP() << endl;
  szLogLine=  LOG0 + "  *setup(): Connected to " + WiFi.localIP();
  LogToBoth(oFBase, acPushPath, szLogLine);

  WriteStringToFirebase(oFBase);

  szLogLine=  LOG0 + "  *setup(): Done.";
  LogToBoth(oFBase, acPushPath, szLogLine);
  return;
}	//setup


void loop() {
}	//loop


void WriteStringToFirebase(Firebase oFBase) {
	//Push a string to Firebase.
	String szPushString= szMakeJSONObject("Name1", "Test String");
	Serial << LOG0 << " WriteStringToFirebase(): Call oFBase.push(" << acPushPath << ", " << szPushString << ")" << endl;
	//szLogLine=  LOG0 + " *WriteStringToFirebase(): Call oFBase.push(" + acPushPath + ", " + szPushString + ")";
	//LogToBoth(oFBase, acPushPath, szLogLine);
	FirebasePush PushCall = oFBase.push(acPushPath, szPushString);
	if (PushCall.error()) {
		Serial << LOG0 << " WriteStringToFirebase(): oFBase.push() call failed, Error: " << PushCall.error().message() << endl;
	}	//if(PushCall.error())
	else {
		// print key.
		Serial << LOG0 << " WriteStringToFirebase(): Call to PushCall.name() returned: " << PushCall.name() << endl;
		//szLogLine=  LOG0 + " *WriteStringToFirebase(): Call to push.name() returned: " + PushCall.name();
		//LogToBoth(oFBase, acPushPath, szLogLine);
	}
/*
	//Get all log entries.
	Serial << LOG0 << " WriteStringToFirebase(): Call fbase.get('/logs')" << endl;
	//szLogLine=  LOG0 + " *WriteStringToFirebase(): Call fbase.get('/logs')";
	//LogToBoth(oFBase, acPushPath, szLogLine);
	FirebaseGet get = oFBase.get("/logs");
	if (get.error()) {
		Serial << LOG0 << " WriteStringToFirebase(): Firebase get failed, Error: " << get.error().message() << endl;
		return;
	}	//if(get.error())
	//Print json.
	Serial << LOG0 << " WriteStringToFirebase(): get.json(): " << get.json() << endl;
	//szLogLine=  LOG0 + " *WriteStringToFirebase(): get.json(): " + get.json();
	//LogToBoth(oFBase, acPushPath, szLogLine);
*/
	return;
}	//WriteStringToFirebase
//Last line.
