// TinyCatCollar.ino
// 7/24/15b Add SD card support from original Flojet.ino
// 7/24/15 Port from Flojet.ino
#include <Arduino.h>
#include <Streaming.h>

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define LOG0 lLineCount++ << " " << millis()

static const int    sGPSPeriodSecs  = 1;
static const long   lMsec           = 1000;
static const long   lGPSPeriodMsec  = sGPSPeriodSecs  * lMsec;

//SD card information
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
static const int sChipSelect            = 4;
static const int sDefaultChipSelectPin  = 10;

//Arduino pins used by the GPS module
static const int GPS_ONOFFPin = A3;
static const int GPS_SYSONPin = A2;
static const int GPS_RXPin    = A1;
static const int GPS_TXPin    = A0;
static const int GPSBaud      = 9600;

static const int sBufferBytes	= 20;

static String			szFilename						= "TGPS724B.TXT";		//Apparantly filenames are limited to 8.3

static long       lLineCount      			= 0;      //Serial Monitor uses for clarity.
static long       lNextReadGPSMsec;          			//Next time to read GPS.
static long       lCurrentMsec;
static boolean    bGPSLoopRunning;         				//loop() checks this

// The GPS connection is attached with a software serial port
SoftwareSerial Gps_serial(GPS_RXPin, GPS_TXPin);

//static byte 			pcGpsBuffer[100];
static byte 			pcGpsBuffer[sBufferBytes + 2];
static int 				sBufferIndex             = 0;
static int 				sLastBufferIndex         = 0;

void setup()  {
  Serial.begin(9600);
  //sWaitForSerialMonitor();
  Serial << LOG0 << " setup(): Begin" << endl;
  sSetupSD();
  //sSetupGPS();		//To be implemented.
  sClearGPSLoop();
  sStartGPSLoop();
  return;
} //setup


void loop()  {
	//if (bGPSLoopRunning && bTimeToReadGPS()) {
	if (bGPSLoopRunning) {
    sStreamGPStoSD();
  } //if(bGPSLoopRunning&&...
  sPrintStatus();
  sCheckKeyboard();
  //sCheckWiFi();			//To be implemented.
  return;
} //loop


int sStreamGPStoSD(){
  //Serial << LOG0 << " sStreamGPStoSD(): Begin" << endl;
	byte cDataByte;
	if (sBufferIndex < sLastBufferIndex) {
		Serial << LOG0 <<" sStreamGPStoSD(): ERROR: BufferIndex= "<< sBufferIndex
		       <<" less than LastIndex= "<< sLastBufferIndex << endl;
	}	//if(sBufferIndex<sLastBufferIndex)

	if (Gps_serial.available()) {
		cDataByte= Gps_serial.read();
		//Serial.write(cDataByte);
		//Serial.println("loop() wrting to SD card");
		pcGpsBuffer[sBufferIndex++]= cDataByte;

		//if(sBufferIndex >= 100) {
		if(sBufferIndex >= sBufferBytes) {
			//Serial.println("loop() Writing 100 bytes to SD card");
  		Serial << LOG0 <<" sStreamGPStoSD(): Writing "<< sBufferBytes <<" bytes to SD card"<< endl;
			sBufferIndex = 0;
			sLastBufferIndex= sBufferIndex;
			//File dataFile = SD.open("TGPS724A.txt", FILE_WRITE);
			File dataFile= SD.open(szFilename, FILE_WRITE);

			// if the file is available, write to it:
			if (dataFile) {
				dataFile.write(pcGpsBuffer, 100);
				dataFile.close();
			} //if(dataFile)
			// if the file isn't open, pop up an error:
			else {
				//Serial.println("Error opening GPS data file");
  			Serial << LOG0 << " sStreamGPStoSD(): Error opening GPS data file" << endl;
			} //if(dataFile)else
		} //if(sBufferIndex>=100)
	} //if(Gps_serial.available())
	else {
		Serial << LOG0 << " sStreamGPStoSD(): No data available at GPS" << endl;
	} //if(Gps_serial.available())else

	sLastBufferIndex= sBufferIndex;
  return 1;
}  //sStreamGPStoSD


int sSetupSD(){
  Serial << lLineCount++ <<" sSetupSD(): Initializing SD card"<< endl;
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(sDefaultChipSelectPin, OUTPUT);

  // see if the card is present and can be initialized:
  if (SD.begin(sChipSelect)) {
    Serial << LOG0 <<" sSetupSD(): SD card is setup"<< endl;
  }
  else {
    Serial << LOG0 <<" sSetupSD(): SD card init failed, or not present"<< endl;
  }
  return 1;
}  //sSetupSD


int sWriteGPStoSD(){
  String  szLogLine= "";
  szLogLine += String(lCurrentMsec);
  szLogLine += ", ";
  szLogLine += String(lLineCount);
  szLogLine += " TinyCatCollar test";

  File LogFile= SD.open(szFilename, FILE_WRITE);
  if (LogFile) {
    Serial << LOG0 << " sWriteGPStoSD(): Write |"<< szLogLine <<"| to " << szFilename << endl;
    LogFile.println(szLogLine);
    LogFile.close();
  }
  else {
    Serial << LOG0 << " sWriteGPStoSD(): error opening " << szFilename << endl;
  }
  return 1;
}  //sWriteGPStoSD


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
        sStreamGPStoSD();
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