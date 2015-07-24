// Sketch to use relays 1 and 2 in parallel to power FloJet on and off
// 7/17B Remove SD code and simplify on Ace
// 7/16 Move development from Sammy XP to Ace W7 because Sammy was crashing Arduno IDE.
// 7/14/15 Remove check on motor voltage and add check on pressure switch.Ifdef SD card out.
#include <Arduino.h>
#include <Streaming.h>

#define LOG0 lLineCount++ << " " << millis()

static const int    asRelay[]       = {0, 7, 6, 5, 4};  //Relay can be 1 to 4,no zero relay
static const int    sPressurePin    = 3;
static const int    sFirstRelay     = 1;
static const int    sLastRelay      = 2;
static const int    sPumpOnSecs     = 360;
static const int    sPumpOffSecs    = 600;
static const long   lMsec           = 1000;
static const long   lPumpOnMillis   = sPumpOnSecs  * lMsec;
static const long   lPumpOffMillis  = sPumpOffSecs * lMsec;

static long       lLineCount      = 0;      //Serial Monitor uses for clarity.
static int        sLastToggleSecsLeft;
static long       lNextReadGPSMsec;          //Next time to toggle pump relay.
static long       lCurrentMsec;

boolean           bPumpIsOn;                //Indicates current state of relays.
boolean           bGPSLoopRunning;         //loop() checks this
boolean           bPumpJustToggled;         //For logging.

/*
When board powers up pump is tuned off and pump loop is not running.
Sketch uses Serial Monitor on a PC for output and input.
Three keys may be pushed: R(un), S(top), T(oggle)
Pressing R starts the pump, sets a time for toggling it off and starts the pump loop running.
Pressing S stops the pump and turns off the pump loop.
Pressing T toggles the pump on or off. If the pump loop is running it sets the toggle time
and keeps the loop running. It does not start the loop if it's not on.
*/

void setup()  {
  Serial.begin(9600);
  sWaitForSerialMonitor();
  Serial << LOG0 << " setup(): Begin" << endl;
  //sSetupSD();			//To be implemented.
 // sSetupGPS();		//To be implemented.
  sClearGPSLoop();
  sStartGPSLoop();
  return;
} //setup


int sWaitForSerialMonitor(){
  //The following (for Leonardo only) waits for SerialMonitor to start on PC.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  return 1;
}  //sWaitForSerialMonitor


void loop()  {
	if (bGPSLoopRunning && bTimeToReadGPS()) {
    sReadGPS();
  } //if(bGPSLoopRunning&&...

  sPrintStatus();
  sCheckKeyboard();
  //sCheckWiFi();			//To be implemented.
  return;
} //loop


int sClearGPSLoop(){
  lNextReadGPSMsec   = 0;
  bGPSLoopRunning  = false;
  return 1;
}  //sClearGPSLoop


int sStartGPSLoop(){
	sClearGPSLoop();
	bGPSLoopRunning= true;
	return 1;
}  //sStartGPSLoop


int sStopGPSLoop(){
	bGPSLoopRunning= false;
	return 1;
}  //sStopGPSLoop


int sCheckKeyboard(){
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
      case 'r':
      case 'R':
        sStartGPSLoop();
        break;
      case 's':
      case 'S':
        sStopGPSLoop();
        break;
      case 't':
      case 'T':
        sReadGPS();
        break;
      default:
        break;
    } //switch
  } //if(Serial.available()
  return 1;
}  //sCheckKeyboard


boolean bTimeToReadGPS(){
  static int  sLastToggleSecsLeft = 0;
  boolean     bReadGPS;
  if (lCurrentMsec > lNextReadGPSMsec) {
    Serial << LOG0 << " bTimeToReadGPS(): lNextReadGPSMsec= "
           << lNextReadGPSMsec << endl;
    Serial << LOG0 <<" bTimeToReadGPS(): Setting bReadGPS to TRUE"<< endl;
    bReadGPS= true;
  } //if(lCurrentMsec>lNextReadGPSMsec)
  else {
    bReadGPS= false;
  } //else
  return bReadGPS;
}  //bTimeToReadGPS


int sPrintStatus(){
  //Print once a second.
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
    if (bGPSLoopRunning) {
      int lSecToToggle= (lNextReadGPSMsec-lCurrentMsec)/1000;
      Serial <<"Seconds until pump toggle= "<< (lSecToToggle / 60) <<":"<< (lSecToToggle % 60) << endl;
    } //if(bGPSLoopRunning)
    else {
      int lSecSinceStart= lCurrentMsec/1000;
      Serial <<"Seconds since start= "<< (lSecSinceStart / 60) <<":"<< (lSecSinceStart % 60) << endl;
    } //if(bGPSLoopRunning)else
  } //if(((lCurrentMsec...
  return 1;
}  //sPrintStatus


int sReadGPS(){
  Serial << LOG0 << " sReadGPS(): Begin" << endl;
  return 1;
}  //sReadGPS
// Last line.