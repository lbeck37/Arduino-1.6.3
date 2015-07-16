// 7/14/15A Flojet.ino sketch to use relays 1 and 2 in parallel to power FloJet on and off
// 7/14/15 Remove check on motor voltage and add check on pressure switch.Ifdef SD card out.
#include <Arduino.h>
#include <Streaming.h>
#ifdef USE_SD
  #include <SPI.h>
  #include <SD.h>
#endif


static const int    asRelay[]       = {0, 7, 6, 5, 4};  //Relay can be 1 to 4,no zero relay
static const int    sPressurePin    = 3;
static const int    sFirstRelay     = 1;
static const int    sLastRelay      = 2;
static const int    sPumpOnSecs     = 360;
static const int    sPumpOffSecs    = 600;
static const long   lMsec           = 1000;
static const long   lPumpOnMillis   = sPumpOnSecs  * lMsec;
static const long   lPumpOffMillis  = sPumpOffSecs * lMsec;

static const boolean  bStopDryPump    = true;

static long       lLineCount      = 0;      //Serial Monitor uses for clarity.
static int        sLastToggleSecsLeft;
static long       lNextToggleMsec;          //Next time to toggle pump relay.
static long       lCurrentMsec;

boolean           bPumpIsOn;                //Indicates current state of relays.
boolean           bPumpLoopRunning;         //loop() checks this
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
  Serial << lLineCount++ << " setup(): Begin" << endl;
  sSetupPumpRelays();
  sClearPumpLoop();
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
  //We will check the pump voltage to make sure pump is not dry but first we
  //will let it toggle if it's time
#if 0
  if (bPumpLoopRunning && bTimeToTogglePump()) {
    sTogglePump();
  }
#endif
  if (bPumpLoopRunning) {
    if (bTimeToTogglePump()) {
      sTogglePump();
    } //if (bTimeToTogglePump())

    if(bStopDryPump) {
      sStopPumpIfDry();
    } //if(bStopDryPump)
  } //if(bPumpLoopRunning)

  sPrintStatus();
  sCheckKeyboard();
  return;
} //loop


int sClearPumpLoop(){
  lNextToggleMsec   = 0;
  bPumpIsOn         = false;
  bPumpLoopRunning  = false;
  bPumpJustToggled  = true;
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


int sSetupPressureSwitch(){
  pinMode(sPressurePin, INPUT);
  //Connect internal pull-up reistor.
  digitalWrite(sPressurePin, HIGH);
  return 1;
}  //sSetupPressureSwitch


boolean bPumpIsDry(){
  boolean bReturn= false;
  int sSwitch= digitalRead(sPressurePin);
  if (sSwitch == LOW) {
    bReturn= true;
  } //if(sSwitch==LOW)
  return bReturn;
}  //bPumpIsDry


int sStopPumpIfDry(){
  //If the pump volts is above the minimum voltage we stop the pump.
  //We say it's above that voltage if the first reading is above and then 2 of the next 3
  //readings are above the minimum. This function will take either 1, 3 or 4 readings
  //We check a maximum of (5) times before returning 0 to indicate pump was
  //not stopped. A 1 is returned to indicate pump was stopped.
  int       sTimesDry = 0;
  int       sCheck    = 0;
  int       sReturn   = 0;
  boolean   bDone     = false;
  boolean   bFirstTime= true;

  while (!bDone) {
    if (sCheck++ < 5) {
      if (bPumpIsDry()) {
      //************Add check on pump switch here*********
        if (sTimesDry++ > 3) {
          sStopPumpLoop();
          sReturn= 1;
          bDone= true;
        } //if(sTimesDry++...
      } //if(bPumpIsDry)
      else {
        //Pressure switch is closed.
        //If this is the first reading then pump is not dry.
        if (bFirstTime) {
          bDone= true;
        } //if(bFirstTime)
        else {
          Serial << lLineCount++ <<" sStopPumpIfDry(): First instance of dry pump"<< endl;
        } //if(bFirstTime)else
      } //if(bPumpIsDry)else
    } //f (sCheck++...
    else {
      bDone= true;
    } //f (sCheck++...else
    bFirstTime= false;
  } //while(!bDone)
  return sReturn;
}  //sStopPumpIfDry


int sCheckKeyboard(){
  if (Serial.available()) {
    char cChar= Serial.read();
    Serial << lLineCount++ << " sCheckKeyboard(): Character read= "<< cChar << endl;
    switch (cChar) {
      case 'r':
      case 'R':
        sStartPumpLoop();
        break;
      case 's':
      case 'S':
        sStopPumpLoop();
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


boolean bTimeToTogglePump(){
  static int  sLastToggleSecsLeft = 0;
  boolean     bTogglePump;
  lCurrentMsec= millis();
  if (lCurrentMsec > lNextToggleMsec) {
    Serial << lLineCount++ << " bTimeToTogglePump(): lCurrentMsec/1000= "<< lCurrentMsec/1000
           <<", lNextToggleMsec/1000= "<< lNextToggleMsec/1000 << endl;
    Serial << lLineCount++ <<" bTimeToTogglePump(): Setting bTogglePump to TRUE"<< endl;
    bTogglePump= true;
  } //if(millis()>lNextToggleMsec)
  else {
    bTogglePump= false;
  } //else
  return bTogglePump;
}  //bTimeToTogglePump


int sPrintStatus(){
  //Print once a second.
  static long lLastPrintedMsec= 0;
  lCurrentMsec= millis();
  if ( ((lCurrentMsec - lLastPrintedMsec)/1000) > 0) {
    lLastPrintedMsec= lCurrentMsec;
    if (bPumpIsOn) {
      Serial << lLineCount++ << " sPrintStatus(): Pump is ON, ";
    }
    else {
      Serial << lLineCount++ << " sPrintStatus(): Pump is OFF, ";
    }
    if (bPumpLoopRunning) {
      int lSecToToggle= (lNextToggleMsec-lCurrentMsec)/1000;
      Serial <<"Seconds until pump toggle= "<< (lSecToToggle / 60) <<":"<< (lSecToToggle % 60) << endl;
    } //if(bPumpLoopRunning)
    else {
      int lSecSinceStart= lCurrentMsec/1000;
      Serial <<"Seconds since start= "<< (lSecSinceStart / 60) <<":"<< (lSecSinceStart % 60) << endl;
    } //if(bPumpLoopRunning)else
  } //if(((lCurrentMsec...
  return 1;
}  //sPrintStatus


#ifdef USE_SD
//SD card information
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
static const int sChipSelect            = 4;
static const int sDefaultChipSelectPin  = 10;
static const int    sMotorVoltsPin  = 0;      //Arduino A0 pin.
static const float  fMotorVoltsConvert= 19.55;  //(4 * 5V *1000 / 1023)
static const float  fMinVoltsToStop   = 12.80;  //Pump is dry if running volts are above this.
static float      fMotorVoltage;
//Reads motor voltage at A0, scaled 4:1, 20V FS => 5V at A0.

int sSetupSD(){
  Serial << lLineCount++ <<" sSetupSD(): Initializing SD card"<< endl;
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(sDefaultChipSelectPin, OUTPUT);

  // see if the card is present and can be initialized:
  if (SD.begin(sChipSelect)) {
    Serial << lLineCount++ <<" sSetupSD(): SD card is setup"<< endl;
  }
  else {
    Serial << lLineCount++ <<" sSetupSD(): SD card init failed, or not present"<< endl;
  }
  return 1;
}  //sSetupSD


int sReadMotorVolts(){
  int sMotorVoltsRaw= analogRead(sMotorVoltsPin);
  fMotorVoltage = (fMotorVoltsConvert * sMotorVoltsRaw) / 1000.0;
  return 1;
}  //sReadMotorVolts


int sLogMotorVolts(){
  String  szLogLine= "";
  szLogLine += String(lCurrentMsec);
  szLogLine += ", ";
  szLogLine += String(lLineCount);
  if (bPumpIsOn) {
    szLogLine += ", ON , ";
  }
  else {
    szLogLine += ", OFF, ";
  }
  szLogLine += String(fMotorVoltage);

  File LogFile= SD.open("PUMPLOG.txt", FILE_WRITE);
  if (LogFile) {
    LogFile.println(szLogLine);
    LogFile.close();
  }
  else {
    Serial << lLineCount++ << " sLogMotorVolts(): error opening " << "PUMPLOG.txt" << endl;
  }
  return 1;
}  //sLogMotorVolts
#endif  //USE_SD


int sTogglePump(){
  lCurrentMsec= millis();
  if (bPumpIsOn) {
    sTurnPumpOn(false);
    lNextToggleMsec= lCurrentMsec + lPumpOffMillis;
  }
  else {
    sTurnPumpOn(true);
    lNextToggleMsec= lCurrentMsec + lPumpOnMillis;
  } //if(bPumpIsOn)else
#if 1
  Serial << lLineCount++ << " sTogglePump(): Current millis= " << lCurrentMsec/1000
         << ", Pump toggled, set next done to " << lNextToggleMsec/1000 << endl;
#else
  int lSecCurrent= lCurrentMsec/1000;
  int lSecToggle= lNextToggleMsec/1000;
  Serial << lLineCount++ << " sTogglePump(): Current millis= "
         << (lSecCurrent / 60) <<":"<< (lSecCurrent % 60)
         << ", Pump toggled, set next done to " << (lSecToggle / 60) <<":"<< (lSecToggle % 60) << endl;
#endif
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
    Serial << lLineCount++ <<" sTurnPumpOn(): Turning pump ON" << endl;
  }
  else {
    bPumpIsOn= false;
    sValue= LOW;
    //bPumpLoopRunning= false;
    Serial << lLineCount++ <<" sTurnPumpOn(): Turning pump OFF" << endl;
  }
  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    sDigitalPin= asRelay[sRelay];
    Serial << lLineCount++ <<" sTurnPumpOn(): Set pin "
           << sDigitalPin << " to " << sValue << endl;
    digitalWrite(sDigitalPin, sValue);    // NO3 and COM3 Connected
  } //for
  return 1;
}  //sTurnPumpOn


int sSetupPumpRelays(){
  Serial << lLineCount++ <<" sSetupPumpRelays(): Begin"<< endl;
  for (int sRelay= sFirstRelay; sRelay <= sLastRelay; sRelay++) {
    int sRelayDigitalPin= asRelay[sRelay];
    Serial << lLineCount++ <<" sSetupPumpRelays(): Set relay #" << sRelay
           << " to pin " << sRelayDigitalPin << endl;
    pinMode(sRelayDigitalPin, OUTPUT);
  } //for
  return 1;
}  //sSetupPumpRelays
// Last line.
