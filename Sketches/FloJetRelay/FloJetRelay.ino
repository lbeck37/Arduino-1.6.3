// 6/28/15b  Sketch to use relays 1 and 2 in parallel to power FloJet on and off

#include <Arduino.h>
#include <Streaming.h>

static const int asRelay[]  = {0, 7, 6, 5, 4};  //Relay can be 1 to 4,no zero relay

static const int  sFirstRelay     = 1;
static const int  sLastRelay      = 2;
static const int  sPumpOnSecs     = 10;
static const int  sPumpOffSecs    = 50;
static const int  sPumpOnMillis   = sPumpOnSecs  * 1000;
static const int  sPumpOffMillis  = sPumpOffSecs * 1000;

static int        sLineCount          = 0;      //Used in outputs to Serial Monitor for clarity.
static int        sToggleSecsLeft;          //Seconds until pump toggles, for Serial Monitor
static int        sLastToggleSecsLeft = 10000;
static long       lNextMillisDone         = 0;      //When the next time is to toggle pump relay.
static long       lCurrentMillis;               //Use inside functions.
static boolean    bPumpIsOn           = false;  //Indicates current state of relays.


void setup()  {
  Serial.begin(9600);
  Serial << sLineCount++ << " setup(): Begin" << endl;

  sSetupPump();
  return;
} //setup


void loop()  {
  if (bTimeToTogglePump()) {
    sTogglePump();
  }
  return;
} //loop


boolean bTimeToTogglePump(){
  boolean bTogglePump= false;

  lCurrentMillis= millis();
  if (lCurrentMillis > lNextMillisDone) {
    bTogglePump= true;
  } //if(millis()>lNextMillisDone)
  else {
    sToggleSecsLeft= (lNextMillisDone - lCurrentMillis) / 1000;
    if (sToggleSecsLeft < sLastToggleSecsLeft) {
      sLastToggleSecsLeft= sToggleSecsLeft;
      Serial << sLineCount++ << " bTimeToTogglePump(): Seconds until pump toggle= "
             << sToggleSecsLeft << endl;
      Serial << sLineCount++ << " bTimeToTogglePump(): Current millis= " << lCurrentMillis
             << ", Next done millis= " << lNextMillisDone << endl;
    } //if
  } //else

  return bTogglePump;
}  //bTimeToTogglePump


int sTogglePump(){
  if (bPumpIsOn) {
    sTurnPumpOn(false);
    bPumpIsOn= false;
    lNextMillisDone= millis() + sPumpOffMillis;
  }
  else {
    sTurnPumpOn(true);
    bPumpIsOn= true;
    lNextMillisDone= millis() + sPumpOnMillis;
  } //if(bPumpIsOn)else

  sLastToggleSecsLeft= 10000;

  return 1;
}  //sTogglePump


int sSetupPump(){
  Serial << sLineCount++ <<" sSetupPump(): Begin"<< endl;
  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    int sRelayDigitalPin= asRelay[sRelay];
    Serial << sLineCount++ <<" sSetupPump(): Set relay #" << sRelay
           << " to pin " << sRelayDigitalPin << endl;
    pinMode(sRelayDigitalPin, OUTPUT);
  } //for
  return 1;
}  //sSetupPump


int sTurnPumpOn(boolean bOn){
  int sDigitalPin;
  int sValue;

  if (bOn) {
    sValue= HIGH;
    Serial << sLineCount++ <<" sTurnPumpOn(): Turning pump ON" << endl;
  }
  else {
    sValue= LOW;
    Serial << sLineCount++ <<" sTurnPumpOn(): Turning pump ON" << endl;
  }

  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    sDigitalPin= asRelay[sRelay];
    Serial << sLineCount++ <<" sTurnPumpOn(): Set pin "
           << sDigitalPin << " to " << sValue << endl;
    digitalWrite(sDigitalPin, sValue);    // NO3 and COM3 Connected
  } //for

  return 1;
}  //sTurnPumpOn
// Last line.