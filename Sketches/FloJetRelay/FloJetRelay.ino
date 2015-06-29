// 6/29/15  Sketch to use relays 1 and 2 in parallel to power FloJet on and off

#include <Arduino.h>
#include <Streaming.h>

static const int asRelay[]  = {0, 7, 6, 5, 4};  //Relay can be 1 to 4,no zero relay

static const int  sFirstRelay     = 1;
static const int  sLastRelay      = 2;
static const int  sPumpOnSecs     = 10;
static const int  sPumpOffSecs    = 50;
static const long lMsec   				= 1000;
static const long lPumpOnMillis   = sPumpOnSecs  * lMsec;
static const long lPumpOffMillis  = sPumpOffSecs * lMsec;

static int        sLineCount          = 0;      //Used in outputs to Serial Monitor for clarity.
static int        sToggleSecsLeft;          //Seconds until pump toggles, for Serial Monitor
static int        sLastToggleSecsLeft;
static long       lNextMillisDone     = 0;      //When the next time is to toggle pump relay.
//static long       lCurrentMillis;               //Use inside functions.
static boolean    bPumpIsOn           = false;  //Indicates current state of relays.
static boolean    bPumpToggled				= true;		//Causes log line to be printed.


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

  long lCurrentMillis= millis();
  if (lCurrentMillis > lNextMillisDone) {
		Serial << sLineCount++ << " bTimeToTogglePump(): lCurrentMillis= " << lCurrentMillis
					 << ", lNextMillisDone= " << lNextMillisDone << endl;
		Serial << sLineCount++ << " bTimeToTogglePump(): Setting bTogglePump to TRUE" << endl;
    bTogglePump= true;
  } //if(millis()>lNextMillisDone)
  else {
    sToggleSecsLeft= (lNextMillisDone - lCurrentMillis) / 1000;
    if (bPumpToggled || (sToggleSecsLeft < sLastToggleSecsLeft)) {
			bPumpToggled= false;
      sLastToggleSecsLeft= sToggleSecsLeft;
      if (bPumpIsOn) {
      	Serial << sLineCount++ << " bTimeToTogglePump(): Pump is ON" << endl;
			}
			else {
      	Serial << sLineCount++ << " bTimeToTogglePump(): Pump is OFF" << endl;
			}
      Serial << sLineCount++ << " bTimeToTogglePump(): Seconds until pump toggle= "
             << sToggleSecsLeft << endl;
#if 0
      Serial << sLineCount++ << " bTimeToTogglePump(): Current millis= " << lCurrentMillis
             << ", Next done millis= " << lNextMillisDone << endl;
#endif
		} //if
  } //else

  return bTogglePump;
}  //bTimeToTogglePump


int sTogglePump(){
  long lCurrentMillis= millis();
  if (bPumpIsOn) {
    sTurnPumpOn(false);
    bPumpIsOn= false;
    lNextMillisDone= lCurrentMillis + lPumpOffMillis;
  }
  else {
    sTurnPumpOn(true);
    bPumpIsOn= true;
    lNextMillisDone= lCurrentMillis + lPumpOnMillis;
  } //if(bPumpIsOn)else

	//Serial << sLineCount++ << " sTogglePump(): lPumpOnMillis= " << lPumpOnMillis
	//			 << ", lPumpOffMillis= " << lPumpOffMillis << endl;

	Serial << sLineCount++ << " sTogglePump(): Current millis= " << lCurrentMillis
				 << ", Setting next done millis= " << lNextMillisDone << endl;
	bPumpToggled= true;
  return 1;
}  //sTogglePump


int sTurnPumpOn(boolean bOn){
  int sDigitalPin;
  int sValue;

  if (bOn) {
    sValue= HIGH;
    Serial << sLineCount++ <<" sTurnPumpOn(): Turning pump ON" << endl;
  }
  else {
    sValue= LOW;
    Serial << sLineCount++ <<" sTurnPumpOn(): Turning pump OFF" << endl;
  }

  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    sDigitalPin= asRelay[sRelay];
    Serial << sLineCount++ <<" sTurnPumpOn(): Set pin "
           << sDigitalPin << " to " << sValue << endl;
    digitalWrite(sDigitalPin, sValue);    // NO3 and COM3 Connected
  } //for

  return 1;
}  //sTurnPumpOn


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

// Last line.