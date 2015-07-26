// Sketch to use relays 1 and 2 in parallel to power FloJet on and off
// 7/26 Work on completing with fill valve on pin 8.
// 7/25 Changed to having Grey Drain and Black Flush cycles instead of just bPumpLoopRunning.
// 7/17B Remove SD code and simplify on Ace
// 7/16 Move development from Sammy XP to Ace W7 because Sammy was crashing Arduno IDE.
// 7/14/15 Remove check on motor voltage and add check on pressure switch.Ifdef SD card out.
#include <Arduino.h>
#include <Streaming.h>

#define LOG0            lLineCount++ << " " << millis()
#define MIN_SEC(Sec)    Serial << (Sec / 60) << ":" << (Sec % 60)
#define CYCLE_SEC       Serial << ((sCycleSec()/60)) <<":"<< (sCycleSec() % 60)

static const int    sPressurePin        = 3;
//Relay pin can be 1 to 4,no zero relay, pin 4 not available, conflicts with SD card.
static const int    asRelay[]           = {0, 7, 6, 5, 4};
static const int    sBlackFillValvePin  = 8;
static const int    sFirstRelay     = 1;
static const int    sLastRelay      = 2;
static const int    sStatusSecs     = 2;
static const int    sTimeoutSecs    = 7 * 60;
static const int    s5GalOnSecs     = 60;
static const long   lMsec           = 1000;
static const long   lStatusMsec     = sStatusSecs  * lMsec;
static const long   lTimeoutMsec    = sTimeoutSecs  * lMsec;
static const long   l5GalOnMsec     = s5GalOnSecs * lMsec;
static const int    sIdleCycle      = 0;
static const int    sGreyDrainCycle = 1;
static const int    sBlackDrainCycle= 2;
static const int    sBlackFillCycle = 3;
static const int    sNumBlackFills= 7;
static const int    sBlackIsDraining= 1;
static const int    sBlackIsFilling = 2;

static long       lLineCount      = 0;      //Serial Monitor uses for clarity.
static long       lNextTimeoutMsec;          //Next time to toggle pump relay.
static long       lNextStatusMsec;          //Next time to print status on Serial Monitor.
static long       lStopBlackFillMsec;       //Time to stop filling black tank
static long       lCurrentMsec;
static long       lCycleStartMsec;
static int        sCurrentCycle;            //sIdleCycle, sGreyDrainCycle, sBlackDrainCycle
static int        sBlackDrainState;
static int        sBlackFillCount;

boolean           bPumpIsOn;                //Indicates current state of relays.

/*
When board powers up pump is tuned off and pump loop is not running.
Sketch uses Serial Monitor on a PC for output and input.
N keys may be pushed: G(rey), B(lack), F(ill), S(top), X(Debug)
G starts the Grey Drain Cycle.
  The pump ia turned on and when the pump runs dry it goes back to the Idle Cycle.
B starts the Black Drain Cycle.
  The pump ia turned on and when the pump runs dry the water fill valve is turned on for
  one minute (~5 gal) and then off and the pump is run until again until dry. This is repeated
  N (7?) times.
S stops any cycle and goes into the Idle Cycle.
F starts the Black Fill Cycle which opens the black tank fill valve for 30 seconds to put
  ~2.5 gal back in the black tank. Use this when finished draining black and want to put some back.
X turns on Debug mode which allows testing without pressure switch being connected.
  Debug mode simulates pump going dry after 5 seconds of Grey or Black Drain Cycle.
Note: The Toggle command is not implemented any more.
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
  sCheckForDryPump();
  sCheckForTimeout();

  sPrintStatus();
  sCheckKeyboard();
  return;
} //loop


int sCycleSec() {
  int sReturn= (lCurrentMsec - lCycleStartMsec) / lMsec;
  return sReturn;
}  //sCycleSec


int sSetCycleStartMsec() {
  lCycleStartMsec= lCurrentMsec;
  Serial << LOG0 <<" sSetCycleStartMsec(): Set  Cycle start to "<< CYCLE_SEC << endl;
  Serial << LOG0 <<" sSetCycleStartMsec(): lCurrentMsec= "<< lCurrentMsec
         << ", lCycleStartMsec= " << lCycleStartMsec << endl;
  int sCycleSecReturns= sCycleSec();
  Serial << LOG0 <<" sSetCycleStartMsec(): sCycleSec() returns "<< sCycleSecReturns << endl;
  return 1;
}  //sSetCycleStartMsec


int sCheckForDryPump() {
  if (bPumpIsOn && bPumpIsDry()) {
    sHandleDryPump();
  }
  return 1;
}  //sCheckForDryPump


int sCheckForTimeout() {
  if (bPumpIsOn && bTimeoutReached()) {
    sStopCycle();
  } //if
  return 1;
}  //sCheckForTimeout


int sClearCycles() {
  Serial << LOG0 <<" sClearCycles(): Begin"<< endl;
  lNextTimeoutMsec   = 0;
  sTurnPumpOn(false);
  sCurrentCycle     = sIdleCycle;
  sSetCycleStartMsec();
  return 1;
}  //sClearCycles


int sStartGreyDrainCycle() {
  Serial << LOG0 <<" sStartGreyDrainCycle(): Begin"<< endl;
  sClearCycles();
  sCurrentCycle= sGreyDrainCycle;
  sSetCycleStartMsec();
  sTurnPumpOn(true);
  return 1;
}  //sStartGreyDrainCycle


int sStartBlackDrainCycle() {
  Serial << LOG0 <<" sStartBlackDrainCycle(): Begin"<< endl;
  sClearCycles();
  sCurrentCycle= sBlackDrainCycle;
  sBlackDrainState= sBlackIsDraining;
  sSetCycleStartMsec();
  sBlackFillCount= 0;
  sTurnPumpOn(true);
  return 1;
}  //sStartBlackDrainCycle


int sStopBlackDrainCycle() {
  Serial << LOG0 <<" sStopBlackDrainCycle(): Stopping Black Drain Cycle"<< endl;
  sStopCycle();
  return 1;
}  //sStopBlackDrainCycle


int sSwitchBlackToFilling() {
  Serial << LOG0 <<" sSwitchBlackToFilling(): Begin"<< endl;
  //Returns 0 if number of desired fills was reached.
  int sReturn= 1;
  if (sBlackFillCount++ < sNumBlackFills) {
    Serial << LOG0 <<" sSwitchBlackToFilling(): Perform fill "<< sBlackFillCount
           <<" of "<< sNumBlackFills << endl;
    sBlackDrainState= sBlackIsFilling;
    lStopBlackFillMsec= millis() + l5GalOnMsec;
    //Implement turning valve on.
  } //if(sBlackFillCount++<sNumBlackFills)
  else {
    Serial << LOG0 <<" sSwitchBlackToFilling(): Total Fills reached: "<< sNumBlackFills << endl;
    sReturn= 0;
    } //if(sBlackFillCount++<sNumBlackFills)else
  return sReturn;
}  //sSwitchBlackToFilling


int sSwitchBlackToDraining() {
  Serial << LOG0 <<" sSwitchBlackToDraining(): Begin"<< endl;
  sBlackDrainState= sBlackIsDraining;
  sSetCycleStartMsec();
  sTurnPumpOn(true);
  //Implement turning valve on.
  return 1;
}  //sSwitchBlackToDraining


int sStartBlackFillCycle() {
  Serial << LOG0 <<" sStartBlackFillCycle(): Begin"<< endl;
  //Fill for 30 seconds for 2.5 gallons.
  sClearCycles();
  sCurrentCycle= sBlackFillCycle;
  sSetCycleStartMsec();
  lStopBlackFillMsec= millis() + (l5GalOnMsec /2);
  sTurnPumpOn(true);
  return 1;
}  //sStartBlackFillCycle


int sStopCycle() {
  Serial << LOG0 <<" sStopCycle(): Begin"<< endl;
  sSetCycleStartMsec();
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
    case sGreyDrainCycle:
      sStopCycle();
      break;
    case sBlackDrainCycle:
      if (sSwitchBlackToFilling() == 0) {
      } //if (sSwitchBlackToFilling() == 0)
      break;
    case sIdleCycle:
    case sBlackFillCycle:
      //Do nothing.
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
      default:
        break;
    } //switch
  } //if(Serial.available()
  return 1;
}  //sCheckKeyboard


boolean bTimeoutReached() {
  boolean     bStopPump= false;
  if (lCurrentMsec > lNextTimeoutMsec) {
    Serial << LOG0 <<" bTimeoutReached(): Timeout reached, lNextTimeoutMsec= "<< lNextTimeoutMsec << endl;
    bStopPump= true;
  } //if(lCurrentMsec>lNextTimeoutMsec)
  return bStopPump;
}  //bTimeoutReached


boolean bTimeToStopFillingBlack() {
  boolean     bStopFilling= false;
  if (lCurrentMsec > lStopBlackFillMsec) {
    Serial << LOG0 <<" bTimeToStopFillingBlack(): lStopBlackFillMsec= "<< lStopBlackFillMsec << endl;
    bStopFilling= true;
  } //if(lCurrentMsec>...
  return bStopFilling;
}  //bTimeToStopFillingBlack


int sPrintStatus() {
  int sSecSinceStart;
  int sSecToToggle;
  static long lLastPrintedMsec= 0;
  lCurrentMsec= millis();
  if (lCurrentMsec >= lNextStatusMsec) {
    lNextStatusMsec= lCurrentMsec + lStatusMsec;
    if (bPumpIsOn) {
      Serial << LOG0 << " sPrintStatus(): Pump is ON, ";
    }
    else {
      Serial << LOG0 << " sPrintStatus(): Pump is OFF, ";
    }
    switch (sCurrentCycle) {
      case sIdleCycle:
        sSecSinceStart= lCurrentMsec/1000;
        Serial <<"Seconds since cycle start= "<< CYCLE_SEC <<", Cycle= IDLE" << endl;
        break;
      case sGreyDrainCycle:
        sSecToToggle= (lNextTimeoutMsec-lCurrentMsec)/1000;
        Serial << "Seconds since cycle start= "<< CYCLE_SEC << " Cycle= GREY DRAIN" << endl;
        break;
      case sBlackDrainCycle:
        sSecToToggle= (lNextTimeoutMsec-lCurrentMsec)/1000;
        Serial <<"Seconds since cycle start= "<< CYCLE_SEC << " Cycle= BLACK FLUSH" << endl;
        break;
      case sBlackFillCycle:
        sSecToToggle= (lNextTimeoutMsec-lCurrentMsec)/1000;
        Serial <<"Seconds since cycle start= "<< CYCLE_SEC << " Cycle= BLACK FILL" << endl;
        break;
      default:
        Serial << LOG0 << "loop(): Bad case in switch()= "<< sCurrentCycle << endl;
        break;
    } //switch
  } //if(lCurrentMsec>=lNextStatusMsec)
  return 1;
}  //sPrintStatus


int sTurnPumpOn(boolean bOn){
  int sDigitalPin;
  int sValue;
  if (bOn) {
    bPumpIsOn= true;
    sValue= HIGH;
    //Serial << LOG0 <<" sTurnPumpOn(): DEBUG ONLY setting sValue= LOW" << endl;
    //sValue= LOW;
    lNextTimeoutMsec= millis() + lTimeoutMsec;
    Serial << LOG0 <<" sTurnPumpOn(): Turning pump ON" << endl;
  }
  else {
    bPumpIsOn= false;
    sValue= LOW;
    //lNextTimeoutMsec= millis() + lPumpOffMsec;
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
