// Sketch to use relays 1 and 2 in parallel to power FloJet on and off
// 7/25 Changed to having Grey Drain and Black Flush cycles instead of just bPumpLoopRunning.
// 7/17B Remove SD code and simplify on Ace
// 7/16 Move development from Sammy XP to Ace W7 because Sammy was crashing Arduno IDE.
// 7/14/15 Remove check on motor voltage and add check on pressure switch.Ifdef SD card out.
#include <Arduino.h>
#include <Streaming.h>

#define LOG0 lLineCount++ << " " << millis()

static const int    sPressurePin        = 3;
//Relay pin can be 1 to 4,no zero relay, pin 4 not available, conflicts with SD card.
static const int    asRelay[]           = {0, 7, 6, 5, 4};
static const int    sBlackFillValvePin  = 8;
static const int    sFirstRelay     = 1;
static const int    sLastRelay      = 2;
static const int    sPumpOnSecs     = 360;
static const int    sPumpOffSecs    = 600;
static const int    s5GalOnSecs     = 60;
static const long   lMsec           = 1000;
static const long   lPumpOnMsec     = sPumpOnSecs  * lMsec;
static const long   lPumpOffMsec    = sPumpOffSecs * lMsec;
static const long   l5GalOnMsec     = s5GalOnSecs * lMsec;
static const int    sIdleCycle      = 0;
static const int    sGreyDrainCycle = 1;
static const int    sBlackDrainCycle= 2;
static const int    sBlackFillCycle = 3;
static const int    sNumBlackFills= 7;
static const int    sBlackIsDraining= 1;
static const int    sBlackIsFilling = 2;

static long       lLineCount      = 0;      //Serial Monitor uses for clarity.
static int        sLastToggleSecsLeft;
static long       lNextToggleMsec;          //Next time to toggle pump relay.
static long       lStopBlackFillMsec;       //Time to stop filling black tank
static long       lCurrentMsec;
static int        sCurrentCycle;            //sIdleCycle, sGreyDrainCycle, sBlackDrainCycle
static int        sBlackDrainState;
static int        sBlackFillCount;

boolean           bPumpIsOn;                //Indicates current state of relays.

/*
When board powers up pump is tuned off and pump loop is not running.
Sketch uses Serial Monitor on a PC for output and input.
N keys may be pushed: G(rey), B(lack), F(ill), S(top), T(oggle)
G starts the Grey Drain Cycle.
  The pump ia turned on and when the pump runs dry it goes back to the Idle Cycle.
B starts the Black Flush Cycle.
  The pump ia turned on and when the pump runs dry the water fill valve is turned on for
  one minute (~5 gal) and then off and the pump is run until again until dry. This is repeated
  N (7?) times.
S stops the pump and goes into the Idle Cycle.
F opens the black tank fill valve for 30 seconds to put ~2.5 gal back in the black tank.
T toggles the pump on or off. If the Grey Drain or Black is running it sets the toggle time
and keeps the loop running. It does not start the loop if it's not on.
In the Grey Drain and Black Flush cycles, a time limit for continuous running of the pump is set,
and when that time is exceeded the pump is shut off and we change to the Idle Cycle.
*/

void setup()  {
  Serial.begin(9600);
  sWaitForSerialMonitor();
  Serial << LOG0 << " setup(): Begin" << endl;
  sSetupPressureSwitch();
  sSetupPumpRelays();
  sClearCycles();
  return;
} //setup


int sWaitForSerialMonitor() {
  //The following (for Leonardo only) waits for SerialMonitor to start on PC.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  return 1;
}  //sWaitForSerialMonitor


void loop()  {
  if (bPumpIsOn && bPumpIsDry()) {
    //sTogglePump();
    sHandleDryPump();
  }
  switch (sCurrentCycle) {
    case sIdleCycle:
      //Do nothing.
      break;
    case sGreyDrainCycle:
      if (bTimeToStopPump()) {
        sStopCycle();
      } //if(bTimeToStopPump())
      break;
    case sBlackDrainCycle:
      if (bTimeToStopFillingBlack()) {
      }
      break;
    case sBlackFillCycle:
      if (bTimeToStopFillingBlack()) {
        sStopCycle();
      }
      break;
    default:
      Serial << LOG0 << "loop(): Bad case in switch()= "<< sCurrentCycle << endl;
      break;
  } //switch

  sPrintStatus();
  sCheckKeyboard();
  return;
} //loop


int sClearCycles() {
  lNextToggleMsec   = 0;
  sTurnPumpOn(false);
  sCurrentCycle     = sIdleCycle;
  return 1;
}  //sClearCycles


int sStartGreyDrainCycle() {
  sClearCycles();
  sCurrentCycle= sGreyDrainCycle;
  sTurnPumpOn(true);
  return 1;
}  //sStartGreyDrainCycle


int sStartBlackDrainCycle() {
  sClearCycles();
  sCurrentCycle= sBlackDrainCycle;
  sBlackDrainState= sBlackIsDraining;
  sBlackFillCount= 0;
  //lStopBlackFillMsec= millis() + l5GalOnMsec;
  sTurnPumpOn(true);
  return 1;
}  //sStartBlackDrainCycle


int sSwitchBlackToFilling() {
  if (sBlackFillCount++ < sNumBlackFills) {
    sBlackDrainState= sBlackIsFilling;
    lStopBlackFillMsec= millis() + l5GalOnMsec;
    //Implement turning valve on.
  }
  return 1;
}  //sSwitchBlackToFilling


int sSwitchBlackToDraining() {
  sBlackDrainState= sBlackIsDraining;
  sTurnPumpOn(true);
  //Implement turning valve on.
  return 1;
}  //sSwitchBlackToDraining


int sStartBlackFillCycle() {
  //Fill for 30 seconds for 2.5 gallons.
  sClearCycles();
  sCurrentCycle= sBlackFillCycle;
  lStopBlackFillMsec= millis() + (l5GalOnMsec /2);
  sTurnPumpOn(true);
  return 1;
}  //sStartBlackFillCycle


int sStopCycle() {
  sCurrentCycle= sIdleCycle;
  sTurnPumpOn(false);
  return 1;
}  //sStopCycle


int sSetupPressureSwitch() {
  pinMode(sPressurePin, INPUT);
  //Connect internal pull-up reistor.
  digitalWrite(sPressurePin, HIGH);
  return 1;
}  //sSetupPressureSwitch


boolean bPumpIsDry() {
  boolean bReturn= false;
  long lNowMsec= millis();
  int sSwitch= digitalRead(sPressurePin);
  if (sSwitch == LOW) {
    Serial << LOG0 <<" bPumpIsDry(): Pressure is low"<< endl;
    bReturn= true;
  } //if(sSwitch==LOW)
  return bReturn;
}  //bPumpIsDry


int sHandleDryPump() {
  sTurnPumpOn(false);
  switch (sCurrentCycle) {
    case sIdleCycle:
      //Do nothing.
      break;
    case sGreyDrainCycle:
      sStopCycle();
      break;
    case sBlackDrainCycle:
      sSwitchBlackToFilling();
      break;
    case sBlackFillCycle:
      Serial << LOG0 << "sHandleDryPump(): ERROR: Pump is dry in Black Fill Cycle "
             << sCurrentCycle << endl;
      break;
    default:
      Serial << LOG0 << "sHandleDryPump(): Bad case in switch()= "<< sCurrentCycle << endl;
      break;
  } //switch
  return 1;
}  //sHandleDryPump


int sCheckKeyboard() {
  if (Serial.available()) {
    char cChar= Serial.read();
    int sChar= cChar;
    if (sChar != 13) {
      Serial << LOG0 <<" sCheckKeyboard(): Character read= "<< cChar <<", "<< sChar << endl;
    }
    else {
      Serial << LOG0 <<" sCheckKeyboard(): Character read= CR" << endl;
    }
    switch (cChar) {
      case 'g':
      case 'G':
        if (sCurrentCycle == sIdleCycle) {
          sStartGreyDrainCycle();
        }
        break;
      case 'b':
      case 'B':
        if (sCurrentCycle == sIdleCycle) {
          sStartBlackDrainCycle();
        }
        break;
      case 'f':
      case 'F':
        if (sCurrentCycle == sIdleCycle) {
          sStartBlackFillCycle();
        }
        break;
      case 's':
      case 'S':
        sStopCycle();
        break;
      case 't':
      case 'T':
        sTogglePump();
        break;
      default:
        break;
    } //switch
  } //if(Serial.available()
  return 1;
}  //sCheckKeyboard


boolean bTimeToStopPump() {
  //static int  sLastToggleSecsLeft = 0;
  boolean     bTogglePump= false;
  if (lCurrentMsec > lNextToggleMsec) {
    Serial << LOG0 <<" bTimeToStopPump(): lNextToggleMsec= "<< lNextToggleMsec << endl;
    bTogglePump= true;
  } //if(lCurrentMsec>lNextToggleMsec)
  return bTogglePump;
}  //bTimeToStopPump


boolean bTimeToStopFillingBlack() {
  boolean     bStopFilling= false;
  if (lCurrentMsec > lStopBlackFillMsec) {
    Serial << LOG0 <<" bTimeToStopFillingBlack(): lStopBlackFillMsec= "<< lStopBlackFillMsec << endl;
    bStopFilling= true;
  } //if(lCurrentMsec>...
  return bStopFilling;
}  //bTimeToStopFillingBlack


int sPrintStatus() {
  //Print once a second.
  int lSecSinceStart;
  int lSecToToggle;
  static long lLastPrintedMsec= 0;
  lCurrentMsec= millis();
  if ( ((lCurrentMsec - lLastPrintedMsec)/1000) > 0) {
    lLastPrintedMsec= lCurrentMsec;
    if (bPumpIsOn) {
      Serial << LOG0 << " sPrintStatus(): Pump is ON, ";
    }
    else {
      Serial << LOG0 << " sPrintStatus(): Pump is OFF, ";
    }
    switch (sCurrentCycle) {
      case sIdleCycle:
        lSecSinceStart= lCurrentMsec/1000;
        Serial <<"Seconds since start= "<< (lSecSinceStart / 60) <<":"<< (lSecSinceStart % 60);
        Serial << " Cycle= IDLE" << endl;
        break;
      case sGreyDrainCycle:
        lSecToToggle= (lNextToggleMsec-lCurrentMsec)/1000;
        Serial << "Seconds until pump toggle= "<< (lSecToToggle / 60) <<":"<< (lSecToToggle % 60);
        Serial << " Cycle= GREY DRAIN" << endl;
        break;
      case sBlackDrainCycle:
        lSecToToggle= (lNextToggleMsec-lCurrentMsec)/1000;
        Serial <<"Seconds until pump toggle= "<< (lSecToToggle / 60) <<":"<< (lSecToToggle % 60);
        Serial << " Cycle= BLACK FLUSH" << endl;
        break;
      case sBlackFillCycle:
        lSecToToggle= (lNextToggleMsec-lCurrentMsec)/1000;
        Serial <<"Seconds until pump toggle= "<< (lSecToToggle / 60) <<":"<< (lSecToToggle % 60);
        Serial << " Cycle= BLACK FILL" << endl;
        break;
      default:
        Serial << LOG0 << "loop(): Bad case in switch()= "<< sCurrentCycle << endl;
        break;
    } //switch
  } //if(((lCurrentMsec...
  return 1;
}  //sPrintStatus


int sTogglePump() {
  if (bPumpIsOn) {
    sTurnPumpOn(false);
  }
  else {
    sTurnPumpOn(true);
  } //if(bPumpIsOn)else
  Serial << LOG0 << " sTogglePump(): Pump toggled, set next done to "
         << lNextToggleMsec << endl;
  return 1;
}  //sTogglePump


int sTurnPumpOn(boolean bOn){
  int sDigitalPin;
  int sValue;
  if (bOn) {
    bPumpIsOn= true;
    sValue= HIGH;
    //Serial << LOG0 <<" sTurnPumpOn(): DEBUG ONLY setting sValue= LOW" << endl;
    //sValue= LOW;
    lNextToggleMsec= millis() + lPumpOnMsec;
    Serial << LOG0 <<" sTurnPumpOn(): Turning pump ON" << endl;
  }
  else {
    bPumpIsOn= false;
    sValue= LOW;
    lNextToggleMsec= millis() + lPumpOffMsec;
    Serial << LOG0 <<" sTurnPumpOn(): Turning pump OFF" << endl;
  }
  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    sDigitalPin= asRelay[sRelay];
    Serial << LOG0 <<" sTurnPumpOn(): Set pin "
           << sDigitalPin << " to " << sValue << endl;
    digitalWrite(sDigitalPin, sValue);    // NO3 and COM3 Connected
  } //for
  //Give pressure time to come up.
  if (bPumpIsOn) {
    delay(500);
  } //if(bPumpIsOn())
  return 1;
}  //sTurnPumpOn


int sOpenBlackFillValve(boolean bOn){
  //int sDigitalPin;
  int sValue;
  if (bOn) {
    sValue= HIGH;
    Serial << LOG0 <<" sOpenBlackFillValve(): Open valve" << endl;
  }
  else {
    bPumpIsOn= false;
    sValue= LOW;
    Serial << LOG0 <<" sOpenBlackFillValve(): Close valve" << endl;
  }
  Serial << LOG0 <<" sOpenBlackFillValve(): Set pin "
         << sBlackFillValvePin << " to " << sValue << endl;
  digitalWrite(sBlackFillValvePin, sValue);
  return 1;
}  //sOpenBlackFillValve


int sSetupPumpRelays() {
  Serial << LOG0 <<" sSetupPumpRelays(): Begin"<< endl;
  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    int sRelayDigitalPin= asRelay[sRelay];
    Serial << LOG0 <<" sSetupPumpRelays(): Set relay #" << sRelay
           << " to pin " << sRelayDigitalPin << endl;
    pinMode(sRelayDigitalPin, OUTPUT);
  } //for
  return 1;
}  //sSetupPumpRelays
// Last line.
