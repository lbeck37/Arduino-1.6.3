// TinyCatCollar.ino
// 7/24/15 Port from Flojet.ino
#include <Arduino.h>
#include <Streaming.h>

#define LOG0 lLineCount++ << " " << millis()

static const int    sGPSPeriodSecs  = 1;
static const long   lMsec           = 1000;
static const long   lGPSPeriodMsec  = sGPSPeriodSecs  * lMsec;

static long       lLineCount      	= 0;      //Serial Monitor uses for clarity.
static long       lNextReadGPSMsec;          	//Next time to read GPS.
static long       lCurrentMsec;
static boolean    bGPSLoopRunning;         		//loop() checks this

void setup()  {
  Serial.begin(9600);
  //sWaitForSerialMonitor();
  Serial << LOG0 << " setup(): Begin" << endl;
  //sSetupSD();			//To be implemented.
 // sSetupGPS();		//To be implemented.
  sClearGPSLoop();
  sStartGPSLoop();
  return;
} //setup


void loop()  {
	if (bGPSLoopRunning && bTimeToReadGPS()) {
    sReadGPS();
  } //if(bGPSLoopRunning&&...
  sPrintStatus();
  sCheckKeyboard();
  //sCheckWiFi();			//To be implemented.
  return;
} //loop


int sReadGPS(){
  Serial << LOG0 << " sReadGPS(): Begin" << endl;
  return 1;
}  //sReadGPS


int sClearGPSLoop(){
  lNextReadGPSMsec   = 0;
  bGPSLoopRunning  = false;
  return 1;
}  //sClearGPSLoop


int sStartGPSLoop(){
	sClearGPSLoop();
	bGPSLoopRunning= true;
  sSetNextGPSReadTime();
	return 1;
}  //sStartGPSLoop


int sStopGPSLoop(){
	bGPSLoopRunning= false;
	return 1;
}  //sStopGPSLoop


int sSetNextGPSReadTime(){
  lNextReadGPSMsec= millis() + lGPSPeriodMsec;
	return 1;
}  //sSetNextGPSReadTime


boolean bTimeToReadGPS(){
  static int  sLastToggleSecsLeft = 0;
  boolean     bReadGPS;
  if (lCurrentMsec > lNextReadGPSMsec) {
  	sSetNextGPSReadTime();
    bReadGPS= true;
  } //if(lCurrentMsec>lNextReadGPSMsec)
  else {
    bReadGPS= false;
  } //else
  return bReadGPS;
}  //bTimeToReadGPS


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


int sPrintStatus(){
  //Print once a second.
  static long lLastPrintedMsec= 0;
  lCurrentMsec= millis();
  if ( ((lCurrentMsec - lLastPrintedMsec)/1000) > 0) {
    lLastPrintedMsec= lCurrentMsec;
    if (bGPSLoopRunning) {
   		Serial << LOG0 <<" sPrintStatus(): GPS Loop is Running"<< endl;
   } //if(bGPSLoopRunning)
    else {
      int lSecSinceStart= lCurrentMsec/1000;
      Serial << LOG0 <<" sPrintStatus(): Seconds since start= "<< (lSecSinceStart / 60) <<":"
             << (lSecSinceStart % 60) << endl;
    } //if(bGPSLoopRunning)else
  } //if(((lCurrentMsec...
  return 1;
}  //sPrintStatus


int sWaitForSerialMonitor(){
  //The following (for Leonardo only) waits for SerialMonitor to start on PC.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  return 1;
}  //sWaitForSerialMonitor
// Last line.