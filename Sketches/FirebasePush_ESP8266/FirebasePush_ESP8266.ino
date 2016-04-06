// Copyright 2015 Google Inc.
// FirebasePush_ESP8266 is a sample that push a new timestamp to firebase on each reset
#include <BeckLib.h>
#include <Streaming.h>
#include <Firebase.h>

static const char szSketchName[]  = "FirebasePush_ESP8266.ino";
static const char szFileDate[]    = "Apr 5, 2016A";

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
  Serial << LOG0 << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  //Create Firebase client.
  Serial << LOG0 << " setup(): Call Firebase('" << acDatabaseURL << "').auth('" << acFirebaseSecret << "')" << endl;
  Firebase oFBase = Firebase(acDatabaseURL).auth(acFirebaseSecret);

  //Connect to wifi.
  //Serial << LOG0 << " setup(): Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
  szLogLine=  LOG0 + " setup(): Call WiFi.begin(" + acRouterName + ", " + acRouterPW + ")";
  LogToBoth(oFBase, acPushPath, szLogLine);
  WiFi.begin(acRouterName, acRouterPW);

  //Serial.print("connecting");
  Serial << LOG0 << " setup(): Call WiFi.status(), wait for WL_CONNECTED" << endl;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial << endl << LOG0 << " setup(): Connected to " << WiFi.localIP() << endl;

  WriteFirebase(oFBase);

/*
  //Push the current timestamp to Firebase.
  String szPushString= szMakeJSONObject("Name1", "LarryB");
  Serial << LOG0 << " setup(): Call oFBase.push(" << acPushPath << ", " << szPushString << ")" << endl;
  //FirebasePush push = oFBase.push(acPushPath, acPushJSON);
  FirebasePush push = oFBase.push(acPushPath, szPushString);
  if (push.error()) {
      Serial << LOG0 << " setup(): Firebase push failed, Error: " << push.error().message() << endl;
      return;
  }	//if(push.error())

  // print key.
  Serial << LOG0 << " setup(): Call to push.name() returned: " << push.name() << endl;

  //Get all entries.
  Serial << LOG0 << " setup(): Call fbase.get('/logs')" << endl;
  FirebaseGet get = oFBase.get("/logs");
  if (get.error()) {
      Serial << LOG0 << " setup(): Firebase get failed, Error: " << get.error().message() << endl;
      return;
  }	//if(get.error())
  //Print json.
  Serial << LOG0 << " setup(): get.json(): " << get.json() << endl;
*/
  return;
}	//setup


void loop() {
}	//loop


void WriteFirebase(Firebase oFBase) {
	//Push the current timestamp to Firebase.
	String szPushString= szMakeJSONObject("Name1", "LarryB");
	Serial << LOG0 << " setup(): Call oFBase.push(" << acPushPath << ", " << szPushString << ")" << endl;
	//FirebasePush push = oFBase.push(acPushPath, acPushJSON);
	FirebasePush push = oFBase.push(acPushPath, szPushString);
	if (push.error()) {
		Serial << LOG0 << " setup(): Firebase push failed, Error: " << push.error().message() << endl;
		return;
	}	//if(push.error())

	// print key.
	Serial << LOG0 << " setup(): Call to push.name() returned: " << push.name() << endl;

	//Get all entries.
	Serial << LOG0 << " setup(): Call fbase.get('/logs')" << endl;
	FirebaseGet get = oFBase.get("/logs");
	if (get.error()) {
		Serial << LOG0 << " setup(): Firebase get failed, Error: " << get.error().message() << endl;
		return;
	}	//if(get.error())
	//Print json.
	Serial << LOG0 << " setup(): get.json(): " << get.json() << endl;
	return;
}	//WriteFirebase


/*
String szMakeJSONObject(String szName, String szValue){
  String szJSONObject= "{\"";
  szJSONObject += szName;
  szJSONObject += "\": \"";
  szJSONObject += szValue;
  szJSONObject += "\"}";
  return szJSONObject;
} //szMakeJSONObject
*/


/*
String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szTermString += szTime;
  szHeader += szGetTime(millis());
  //szHeader += " ";
  return szHeader;
} //szLogLineHeader


String szGetTime(long lMsec){
  String  szString;
  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";  //Trailing blank.
  return szString;
} //szGetTime


//szAddLeadingZeros() adds 1 or 2 zeros (depending on sNumDigits being 3 or not).
String szAddZeros(int sValue, int sNumDigits){
  String szReturn;
  if ((sNumDigits == 3) && (sValue < 100)){
    szReturn= "0";
  } //if((sNumDigits==3)&&(sValue<100)
  if (sValue < 10){
    szReturn += "0";
  } //if(lValue<10)
  szReturn += String(sValue);
  return szReturn;
} //szAddZeros
*/
//Last line.
