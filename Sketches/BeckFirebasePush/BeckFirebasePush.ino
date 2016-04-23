// 4/23/16 Was FirebasePush_ESP8266.ino example
static const char acSketchName[]  = "BeckFirebasePush.ino";
static const char acFileDate[]    = "Apr 23, 2016F";

// FirebasePush_ESP8266 is a sample that push a new timestamp to firebase
// on each reset.

#include <BeckLib.h>
#include <Firebase.h>

static const char   	acRouterName[]      = "Aspot24";
static const char   	acRouterPW[]        = "Qazqaz11";
static const String 	acDatabaseURL		= "intense-fire-3958.firebaseio.com";
static const String 	acFirebaseSecret	= "LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe";
String	szLogLine;
// create firebase client.
//Firebase fbase = Firebase("example.firebaseio.com").auth("secret_or_token");
//Firebase fbase = Firebase("intense-fire-3958.firebaseio.com").auth("LhXHxFsUn7SVYoRC82dKKSqqD67Ls9nfdtMBAWUe");
Firebase fbase = Firebase(acDatabaseURL).auth(acFirebaseSecret);

void setup() {
  Serial.begin(115200);
  Serial << endl;
  Serial << LOG0 << " Setup() done in " << acSketchName << ", version: " << acFileDate << endl;

  // connect to wifi.
  //WiFi.begin("SSID", "PASSWORD");
  Serial << LOG0 << " Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
  WiFi.begin(acRouterName, acRouterPW);

  Serial << LOG0 << " Call WiFi.waitForConnectResult()" << endl;
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
	Serial << LOG0 << " WiFi failed, retrying." << endl;
	Serial << LOG0 << " Call WiFi.begin(" << acRouterName << ", " << acRouterPW << ")" << endl;
    WiFi.begin(acRouterName, acRouterPW);
   }

  Serial << LOG0 << " My WiFi IP address= " << WiFi.localIP() << endl;

   // add a new entry.
  FirebasePush push = fbase.push("/logs", "{\".sv\": \"timestamp\"}");
  if (push.error()) {
      Serial.println("Firebase push failed");
      Serial.println(push.error().message());  
   }

  // print key.
  //Serial.println(push.name());
  Serial << LOG0 << " push.name()= " << push.name() << endl;

  Serial << LOG0 << " Call fbase.get(\"/logs\")" << endl;
  // get all entries.
  FirebaseGet get = fbase.get("/logs");
  if (get.error()) {
      Serial.println("Firebase get failed");
      Serial.println(get.error().message());
      return;
  }
  // print json.
  //Serial.println(get.json());
  Serial << LOG0 << " get.json()= " << get.json() << endl;
}

void loop() {
}
