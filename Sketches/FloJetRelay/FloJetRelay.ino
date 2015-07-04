// 7/03/15  Sketch to use relays 1 and 2 in parallel to power FloJet on and off

#include <Arduino.h>
#include <Streaming.h>

static const int asRelay[]  = {0, 7, 6, 5, 4};  //Relay can be 1 to 4,no zero relay

static const int  sFirstRelay     = 1;
static const int  sLastRelay      = 2;
static const int  sPumpOnSecs     = 15;
static const int  sPumpOffSecs    = 60;
static const long lMsec   				= 1000;
static const long lPumpOnMillis   = sPumpOnSecs  * lMsec;
static const long lPumpOffMillis  = sPumpOffSecs * lMsec;

static int        sLineCount          = 0;      //Used in outputs to Serial Monitor for clarity.
static int        sToggleSecsLeft;          		//Seconds until pump toggles, for Serial Monitor
static int        sLastToggleSecsLeft;
static long       lNextToggleMsec;      				//When the next time is to toggle pump relay.
static boolean    bPumpIsOn;  									//Indicates current state of relays.
static boolean    bPumpLoopRunning;							//loop() checks this
static boolean		bPumpJustToggled;							//For logging.

/*
When board powers up pump is tuned off and pump loop is not running.
Sketch uses Serial Monitor on a PC for output and input.
Three keys may be pushed: R(un), S(top), T(oggle)
Pressing R starts the pump, sets a time for toggling it off and starts the pump loop running.
Pressing S stops the pump and turns off the pump loop.
Pressing T toggles the pump on or off, sets the toggle time and keeps the loop running
*/

void setup()  {
  Serial.begin(9600);
  Serial << sLineCount++ << " setup(): Begin" << endl;

  sSetupPumpRelays();
  sClearPumpLoop();
  return;
} //setup


void loop()  {
  if (bPumpLoopRunning && bTimeToTogglePump()) {
    sTogglePump();
  }
  sCheckKeyboard();
  return;
} //loop


int sClearPumpLoop(){
	lNextToggleMsec		= 0;
	bPumpIsOn					= false;
	bPumpLoopRunning	= false;
	bPumpJustToggled	= true;
  return 1;
}  //sClearPumpLoop


int sStartPumpLoop(){
  sClearPumpLoop();
	bPumpLoopRunning= true;
  return 1;
}  //sStartPumpLoop


int sStopPumpLoop(){
		bPumpLoopRunning= false;
		sTurnPumpOn(false);
  return 1;
}  //sStopPumpLoop


int sCheckKeyboard(){
  if (bStopWasPressed()) {
		sStopPumpLoop();
	}	//bStopWasPressed

  if (bRunWasPressed()) {
		sStartPumpLoop();
	}	//bRunWasPressed

  if (bToggleWasPressed()) {
    sTogglePump();
	}	//bRunWasPressed
  return 1;
}  //sCheckKeyboard


boolean bStopWasPressed(){
  if (Serial.available()) {
		char cChar= Serial.read();
		if ((cChar= 's') || (cChar= 'S')) {
			return true;
		}
		else {
			return false;
		}	//if((cChar= ...else
	}	//if(Serial.available())
}  //bStopWasPressed


boolean bRunWasPressed(){
  if (Serial.available()) {
		char cChar= Serial.read();
		if ((cChar= 'r') || (cChar= 'R')) {
			return true;
		}
		else {
			return false;
		}	//if((cChar= ...else
	}	//if(Serial.available())
}  //bRunWasPressed


boolean bToggleWasPressed(){
  if (Serial.available()) {
		char cChar= Serial.read();
		if ((cChar= 't') || (cChar= 'T')) {
			return true;
		}
		else {
			return false;
		}	//if((cChar= ...else
	}	//if(Serial.available())
}  //bToggleWasPressed


boolean bTimeToTogglePump(){
	static int  sLastToggleSecsLeft	= 0;
  boolean 		bTogglePump					= false;

  long lCurrentMsec= millis();
  if (lCurrentMsec > lNextToggleMsec) {
		Serial << sLineCount++ << " bTimeToTogglePump(): lCurrentMsec= " << lCurrentMsec
					 << ", lNextToggleMsec= " << lNextToggleMsec << endl;
		Serial << sLineCount++ << " bTimeToTogglePump(): Setting bTogglePump to TRUE" << endl;
    bTogglePump= true;
  } //if(millis()>lNextToggleMsec)
  else {
		//Print seoonds left every second.
    sToggleSecsLeft= (lNextToggleMsec - lCurrentMsec) / 1000;
    if (bPumpJustToggled || (sToggleSecsLeft < sLastToggleSecsLeft)) {
			bPumpJustToggled= false;
      sLastToggleSecsLeft= sToggleSecsLeft;
      if (bPumpIsOn) {
      	Serial << sLineCount++ << " bTimeToTogglePump(): Pump is ON" << endl;
			}
			else {
      	Serial << sLineCount++ << " bTimeToTogglePump(): Pump is OFF" << endl;
			}
      Serial << sLineCount++ << " bTimeToTogglePump(): Seconds until pump toggle= "
             << sToggleSecsLeft << endl;
		} //if
  } //else

  return bTogglePump;
}  //bTimeToTogglePump


int sTogglePump(){
  long lCurrentMsec= millis();
  if (bPumpIsOn) {
    sTurnPumpOn(false);
    lNextToggleMsec= lCurrentMsec + lPumpOffMillis;
  }
  else {
    sTurnPumpOn(true);
    lNextToggleMsec= lCurrentMsec + lPumpOnMillis;
  } //if(bPumpIsOn)else

	Serial << sLineCount++ << " sTogglePump(): Current millis= " << lCurrentMsec
				 << ", Pump toggled, set next done to " << lNextToggleMsec << endl;
	bPumpJustToggled= true;
  return 1;
}  //sTogglePump


int sTurnPumpOn(boolean bOn){
  int sDigitalPin;
  int sValue;

  if (bOn) {
    bPumpIsOn= true;
    sValue= HIGH;
    //bPumpLoopRunning= true;
    Serial << sLineCount++ <<" sTurnPumpOn(): Turning pump ON" << endl;
  }
  else {
    bPumpIsOn= false;
    sValue= LOW;
    //bPumpLoopRunning= false;
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


int sSetupPumpRelays(){
  Serial << sLineCount++ <<" sSetupPumpRelays(): Begin"<< endl;
  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    int sRelayDigitalPin= asRelay[sRelay];
    Serial << sLineCount++ <<" sSetupPumpRelays(): Set relay #" << sRelay
           << " to pin " << sRelayDigitalPin << endl;
    pinMode(sRelayDigitalPin, OUTPUT);
  } //for
  return 1;
}  //sSetupPumpRelays

// Last line.