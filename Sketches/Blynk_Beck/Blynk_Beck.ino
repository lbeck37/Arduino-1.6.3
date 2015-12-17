static const char szSketchName[]  = "Blynk_Beck.ino";
static const char szFileDate[]    = "Dec 16, 2015S";
// 12/16/15 Implement thermostat for GARAGE version.
// 12/15/15 Remove unused state virtual pins, implement thermostat for GARAGE version.
// 12/14/15 Rearrange virtual pins, build GARAGE version.
// 12/13/15 Merge in support for Fireplace.
// 12/12/15 Created from Blynk_LightTimer.ino

//Uncomment out desired implementation.
//#define FRONT_LIGHTS
//#define FIREPLACE
#define GARAGE

#include <Streaming.h>
#include <Time.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONEWIRE_PIN       12

//Define Virtual Pin names
#define ReadF_V0          V0
#define ReadF_V1          V1
#define SetSetpointF_V2   V2
#define GetSetpointF_V3   V3
#define ThermoSwitch_V4   V4
#define ThermoLED_V5      V5

#define Unassigned_V6     V6

#define Terminal_V7       V7
#define LCD_Line0_V8      V8
#define LCD_Line1_V9      V9

//Relay #1
#define Switch_1V10       V10
#define TimerA_1V11       V11
#define TimerB_1V12       V12
#define LED_1V13          V13

#define Unassigned_V14    V14
//Relay #2
#define Switch_2V15       V15
#define TimerA_2V16       V16
#define TimerB_2V17       V17
#define LED_2V18          V18

#define Unassigned_V19    V19
//Relay #3
#define Switch_3V20       V20
#define TimerA_3V21       V21
#define TimerB_3V22       V22
#define LED_3V23          V23

#define Unassigned_V24    V24
//Relay #4
#define Switch_4V25       V25
#define TimerA_4V26       V26
#define TimerB_4V27       V27
#define LED_4V28          V28

#define Unassigned_V29    V29
#define Unassigned_V30    V30
#define Unassigned_V31    V31

#define LOG0    szLogLineHeader(lLineCount++)

static const int    sSwitchOpen           = HIGH;
static const int    sSwitchClosed         = LOW;
static const int    sOff                  = 0;
static const int    sOn                   = 1;
static const int    sNumRelays            = 2;
static const int    sRelayPin[]           = {4, 5};
//(3) types of sketches are supported: front lights, fireplace and garage
static const int    sFrontLights          = 1;
static const int    sFireplace            = 2;
static const int    sGarage               = 3;
static const int    sOneWirePin           = ONEWIRE_PIN;  //Dallas DS18B20 Temperature Sensor
static const int    sMaxFDelta            = 2;  //Amount room temp can rise above setpoint.

static const long   lMsecPerDay           = 86400000;
static const long   lMsecPerHour          =  3600000;
static const long   lMsecPerMin           =    60000;
static const long   lMsecPerSec           =     1000;

static const int    sThermoRelay      = 0;      //Relay number that turns furnace on and off.
static const long   sThermoReadSpacing= 5000;   //Number of mSec between reads
static const long   sThermoTimesInRow = 3;      //Max times temp is outside range before switch
static const float  fMaxHeatRangeF    = 2.00;   //Temp above setpoint before heat is turned off

//To get Blynk Auth Token from the Blynk App, go to the Project Settings (nut icon).
#ifdef FRONT_LIGHTS
  char acBlynkAuthToken[] = "37a58cc7a39045a59bca1fb1281880a2";     //Light Timer Blynk token
  static const char szSketchType[]    = "FRONT_LIGHTS";
  static int sSketchType= sFrontLights;
#endif
#ifdef FIREPLACE
  char acBlynkAuthToken[] = "35131c5204f34f8e93b574436df46397";
  static const char szSketchType[]    = "FIREPLACE";
  static int sSketchType= sFireplace;
#endif
#ifdef GARAGE
  char acBlynkAuthToken[] = "5e9c5f0ae3f8467597983a6fa9d11101";
  static const char szSketchType[]    = "GARAGE";
  static int sSketchType= sGarage;
#endif

WidgetTerminal      oTerminal(Terminal_V7);
WidgetLCD           LCDWidget(1);
WidgetLED           oLED1(LED_1V13);
WidgetLED           oLED2(LED_2V18);
WidgetLED           oLED3(ThermoLED_V5);

//Maxim/Dallas OneWire sensors
/* Set up a oneWire instance to communicate with any OneWire device*/
OneWire oOneWire(sOneWirePin);

/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature oSensors(&oOneWire);

static int          sRelayState[2];
static long         lLineCount        = 0;      //Serial Monitor uses for clarity.
static long         lLineCount2       = 0;      //For Blynk terminal window.
static long         lNumLoops         = 1;
static float        fLastDegF         = 37.37;  //Last temperature reading.
static int          sSetpointF        = 37;
static int          sThermoTimesCount = 0;      //Number of times temperature out of range
static long         lNextThermoMsec   = 0;
static bool         bThermoOn         = true;  //Whether thermostat is running.
static bool         bHeatIsOn         = false;  //If switch is on to turn on furnace.
static float        fThermoOffDegF    = sSetpointF + fMaxHeatRangeF;

void setup()
{
  sSetupTime();
  Serial.begin(9600);
  Serial << endl << LOG0 << " setup(): Initialized serial to 9600 baud" << endl;
  Serial << LOG0 << " setup(): Sketch: " << szSketchName << "/" << szSketchType << ", " << szFileDate << endl;
  Wire.begin();

  Serial << LOG0 << " setup(): Call Blynk.begin(acBlynkAuthToken, dlinky, Qazqaz11)" << endl;
  Blynk.begin(acBlynkAuthToken, "dlinky", "Qazqaz11");
  Serial << LOG0 << " setup(): Back from Blynk.begin()" << endl;

  Serial << LOG0 << " setup(): sRelayPin[0]= " << sRelayPin[0] << ", sRelayPin[1]= " << sRelayPin[1] << endl;

  /*//Test writing to LCD
  LCDWidget.clear();
  int sCharPos= 0;   //Position 0-15
  int sLineNum= 0;   //Line 0-1
  LCDWidget.print(0, 0, "Wow! :)");*/

  sSetupRelays();
  return;
} //setup


void loop()
{
  Blynk.run();
  sHandleThermostat();
} //loop

int sSetupTime(){
  setTime(0,0,0, 0,0,0);  //hr, min, sec, day, month, year
  return 1;
} //sSetupTime


int sDebugHandleThermostat(float fDegF){
  String szLogString = "sHandleThermostat";
  String szLogString2= " ";
  sLogToBoth(szLogString);
  szLogString= " DegF=";
  sLogToBoth(szLogString, fDegF);
  szLogString= " SetpointF=";
  sLogToBoth(szLogString, sSetpointF);
  szLogString= " OffDegF=";
  sLogToBoth(szLogString, fThermoOffDegF);
  szLogString= " bHeatIsOn=";
  sLogToBoth(szLogString, bHeatIsOn);
  szLogString= " OnCount=";
  sLogToBoth(szLogString, sThermoTimesCount);
  return 1;
} //sDebugHandleThermostat


int sHandleThermostat(){
  if (millis() >= lNextThermoMsec){
    String szLogString = "sHandleThermostat";
    lNextThermoMsec= millis() + sThermoReadSpacing;
    //Only do anything if the thermostat is turned on.
    if (bThermoOn){
      float fCurrentF= fGetDegF(true);
      sDebugHandleThermostat(fCurrentF);
      if (bHeatIsOn){
        if (fCurrentF >= fThermoOffDegF){
          if (++sThermoTimesCount >= sThermoTimesInRow){
            szLogString= " Open Switch";
            sLogToBoth(szLogString);
            sSetThermoSwitch(sSwitchOpen);
            bHeatIsOn= false;
            sThermoTimesCount= 0;
          } //if(sThermoTimesCount>=sThermoTimesInRow)
        } //if(fCurrentF>=fThermoOffDegF)
        else{
          sThermoTimesCount= 0;
        } //if(fCurrentF>=fThermoOffDegF)else
      } //if(bHeatIsOn)
      else{
        if (fCurrentF < sSetpointF){
          if (++sThermoTimesCount >= sThermoTimesInRow){
            szLogString= " Close Switch";
            sLogToBoth(szLogString);
            bHeatIsOn= true;
            sSetThermoSwitch(sSwitchClosed);
            sThermoTimesCount= 0;
          } //if(sThermoTimesCount>=sThermoTimesInRow)
        } //if(fCurrentF<sSetpointF)
        else{
          sThermoTimesCount= 0;
        } //if(fCurrentF<sSetpointF)else
      } //if(bHeatIsOn)else
    } //if(bThermoOn)
    else{
      sLogToBoth(szLogString);
      szLogString= " bThermoOn is false";
      sLogToBoth(szLogString);
    }
  } //if(millis()>=lNextThermoMsec)
  return 1;
} //sHandleThermostat


int sSetThermoState(int sState){
  if (sState == sOn){
    bThermoOn= true;
  } //if(sState==sOn)
  else{
    bThermoOn= false;
    bHeatIsOn= false;
    sThermoTimesCount= 0;
    sSetThermoSwitch(sSwitchOpen);
  } //if(sState==sOn)else
  return 1;
} //sSetThermoState


int sSetThermoSwitch(int sRelaySetting){
  sSetRelay(sThermoRelay, sRelaySetting);
  return 1;
} //sSetThermoSwitch


int sHandleBlynkLEDs(){
  if (sRelayState[0]){
    Serial << LOG0 << " sHandleBlynkLEDs(): Call oLED1.off()" << endl;
    oLED1.off();
  } //if(sRelayState[0])
  else{
    Serial << LOG0 << " sHandleBlynkLEDs(): Call oLED1.on()" << endl;
    oLED1.on();
  } //if(sRelayState[0])else

  if (sRelayState[1]){
    Serial << LOG0 << " sHandleBlynkLEDs(): Call oLED2.off()" << endl;
    oLED2.off();
  } //if(sRelayState[1])
  else{
    Serial << LOG0 << " sHandleBlynkLEDs(): Call oLED2.on()" << endl;
    oLED2.on();
  } //if(sRelayState[1])else

  if (bThermoOn){
    Serial << LOG0 << " sHandleBlynkLEDs(): Call oLED3.on()" << endl;
    oLED3.on();
  } //if(bThermoOn)
  else{
    Serial << LOG0 << " sHandleBlynkLEDs(): Call oLED3.off()" << endl;
    oLED3.off();
  } //if(bThermoOn)else
  return 1;
} //sHandleBlynkLEDs


String szGetTime(long lMsec){
  //long    lMsecNow  = millis();
  String  szString;

  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";     //Send with trailing blank to seperate from next field.
  return szString;
} //szGetTime


//szAddLeadingZeros() adds 1 or 2 zeros (depending on sNumDigits being 3 or not).
String szAddZeros(int sValue, int sNumDigits){
  String szReturn;
  if ((sNumDigits == 3) && (sValue < 100)){
    szReturn= "0";
  } //if((sNumDigits==3)&&(sValue<100)
  if (sValue < 10){
    szReturn += "0";
  } //if(lValue<10)
  szReturn += String(sValue);
  return szReturn;
} //szAddZeros

/*
int sTerminalPrintVersion(){
  Serial << LOG0 << " sTerminalPrintVersion: Begin" << endl;
  oTerminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  oTerminal.println("-------------");
  oTerminal.println("Type 'Marco' and get a reply, or type");
  oTerminal.println("anything else and get it printed back.");
  oTerminal.flush();
  return 1;
} //sTerminalPrintVersion
*/

// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
//int sWriteTerminalLine(char *szString){
int sWriteTerminalLine(String szString){
  oTerminal.println(szString) ;
  oTerminal.flush();          // Ensure everything is sent
  return 1;
} //sWriteTerminalLine


int sWriteTerminalString(String szString){
  oTerminal.print(szString);
  oTerminal.print(" ");       //Send training space
  oTerminal.flush();          // Ensure everything is sent
  return 1;
} //sWriteTerminalString


String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szTermString += szTime;
  szHeader += szGetTime(millis());
  //szHeader += " ";
  return szHeader;
} //szLogLineHeader


//sLogToBoth() and sBlynkLogLine()have multiple versions
//depending on there being a 2nd variable and its type.
int sLogToBoth(String szLogString){
  Serial << LOG0 << szLogString << endl;
  sBlynkLogLine(szLogString);
  return 1;
} //sLogToBoth:empty


int sLogToBoth(String szLogString, String szLogValue){
  Serial << LOG0 << szLogString << " " << szLogValue << endl;
  sBlynkLogLine(szLogString, szLogValue);
  return 1;
} //sLogToBoth:String


int sLogToBoth(String szLogString, int sLogValue){
  Serial << LOG0 << szLogString << " " << sLogValue << endl;
  sBlynkLogLine(szLogString, sLogValue);
  return 1;
} //sLogToBoth:int


int sLogToBoth(String szLogString, float fLogValue){
  Serial << LOG0 << szLogString << " " << fLogValue << endl;
  sBlynkLogLine(szLogString, fLogValue);
  return 1;
} //sLogToBoth:float


int sBlynkLogLine(String szString){
  String szTermString= szLogLineHeader(lLineCount2++);
  szTermString += szString;
  sWriteTerminalLine(szTermString);
  return 1;
} //sBlynkLogLine:empty


int sBlynkLogLine(String szString, String szLogValue){
  String szTermString= szLogLineHeader(lLineCount2++);
  szTermString += szString;
  szTermString +=  " ";
  szTermString +=  szLogValue;
  sWriteTerminalLine(szTermString);
  return 1;
} //sBlynkLogLine:String


int sBlynkLogLine(String szString, int sValue){
  String szTermString= szLogLineHeader(lLineCount2++);
  szTermString += szString;
  szTermString +=  " ";
  szTermString +=  sValue;
  sWriteTerminalLine(szTermString);
  return 1;
} //sBlynkLogLine:int


int sBlynkLogLine(String szString, float fValue){
  String szTermString= szLogLineHeader(lLineCount2++);
  szTermString += szString;
  szTermString +=  fValue;
  sWriteTerminalLine(szTermString);
  return 1;
} //sBlynkLogLine:float


int sSetupRelays(){
  //Serial << LOG0 << " sSetupRelays: Call pinMode() & sSetRelay() to enable OUTPUT on both pins" << endl;
  for (int sRelay= 0; sRelay < sNumRelays; sRelay++){
    pinMode(sRelayPin[sRelay], OUTPUT);
    sSetRelay(sRelay, sSwitchOpen);
  } //for
  return 1;
} //sSetupRelays


int sSetRelay(int sRelay, int sRelaySetting){
  //Serial << LOG0 << " sSetRelay: Call digitalWrite to set relay on Pin " << sRelayPin[sRelay] << " to " << sRelaySetting << endl;
  String szLogString= "Set Switch ";
  szLogString += sRelay;
  szLogString += " to ";
  szLogString += !sRelaySetting;
  sLogToBoth(szLogString);
  digitalWrite(sRelayPin[sRelay], sRelaySetting);
  sRelayState[sRelay]= sRelaySetting;
  sHandleBlynkLEDs();
  return 1;
} //sSetRelay


float fGetDegF(bool bTakeReading){
  float fDegFReturn= 37.37;   //Value used for default in testing w/o reading sensor. fLastDegF
  if (bTakeReading){
    oSensors.requestTemperatures(); // Send the command to get temperatures
    fDegFReturn= oSensors.getTempFByIndex(0);
    fLastDegF= fDegFReturn;
  } //if(bTakeReading)
  else{
    fDegFReturn= fLastDegF;
  } //if(bTakeReading)else
  return fDegFReturn;
}  //fGetDegF


float fRound(float fNum){
  oSensors.requestTemperatures(); // Send the command to get temperatures
  float fRounded= floor(fNum + 0.5);
  return fRounded;
}  //fRound


int sSendIntToBlynk(int sVirtualPin, int sValue){
  String szString= " sSendIntToBlynk: ";
  sBlynkLogLine(szString, sValue);
  Blynk.virtualWrite(sVirtualPin, sValue);
  return 1;
} //sSendIntToBlynk


//BLYNK_READ() functions are called by the Blynk app on the phone (at a 1 second interval)
//and returns the value or state of some variable.
//BLYNK_WRITE() functions are called by the Blynk app on the phone
//and pass a variable in the "param" object.
BLYNK_READ(ReadF_V0){
  bool bTakeReading= true;
  float fDegF= fGetDegF(bTakeReading);
  String szLogString= "Read ReadF_V0 ";
  sLogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V0, fRound(fDegF));
} //BLYNK_READ(ReadF_V0)


BLYNK_READ(ReadF_V1){
  bool bTakeReading= false;
  float fDegF= fGetDegF(bTakeReading);
  String szLogString= "Read ReadF_V1 ";
  sLogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V1, fDegF);
} //BLYNK_READ(ReadF_V1)


BLYNK_WRITE(SetSetpointF_V2){
  int sThermoSetting= param.asInt();
  sSetpointF= sThermoSetting;
  fThermoOffDegF= sSetpointF + fMaxHeatRangeF;
  String szLogString= "SetSetpointF_V2 ";
  sLogToBoth(szLogString, sSetpointF);

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  sSendIntToBlynk(GetSetpointF_V3, sSetpointF);
  return;
} //BLYNK_WRITE(Switch_2V15)


BLYNK_READ(GetSetpointF_V3){
  int sReturnF= sSetpointF;
  String szLogString= "GetSetpointF_V3 ";
  sLogToBoth(szLogString, sSetpointF);

  Blynk.virtualWrite(GetSetpointF_V3, sReturnF);
} //BLYNK_READ(GetSetpointF_V3)


BLYNK_WRITE(ThermoSwitch_V4){
  //Turn thermostat on and off.
  int sParam= param.asInt();
  String szLogString= "ThermoSwitch_V4 ";
  sLogToBoth(szLogString, sParam);
  sSetThermoState(sParam);

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  sSendIntToBlynk(GetSetpointF_V3, sSetpointF);

  sHandleBlynkLEDs();
  return;
} //BLYNK_WRITE(ThermoSwitch_V4)


//Handler callback function called when Button set as a Switch is pressed.
//Light around button is lit when 1 is passed as parameter, unlit when 0 is passed.
//Opto-isolated relay is inverse logic, pulling input pin low cn relay.
//Relay #0 is connected to Blynk virtual pins 10, 11, 12
//Relay #1 is connected to Blynk virtual pins 20, 21, 22
BLYNK_WRITE(Switch_1V10){
  int sSetting= param.asInt();
  int sRelaySetting;
  String szLogString= "Set Switch_1V10 ";
  szLogString += sRelaySetting;
  sLogToBoth(szLogString);
/*
  //Test writing to LCD
  LCDWidget.clear();
  int sCharPos= 0;   //Position 0-15
  int sLineNum= 0;   //Line 0-1
  LCDWidget.print(0, 0, "Relay #0 set to: ");
*/
  if (sSetting == 1){
    sRelaySetting= sSwitchClosed;
  }
  else{
    sRelaySetting= sSwitchOpen;
  }
  sSetRelay(0, sRelaySetting);    //Set relay #0
  return;
} //BLYNK_WRITE(Switch_1V10)


BLYNK_WRITE(TimerA_1V11){
  int sSetting= param.asInt();
  int sRelaySetting;
  String szLogString= "Set TimerA_1V11 ";
  szLogString += sRelaySetting;
  sLogToBoth(szLogString);

  if (sSetting == 1){
    sRelaySetting= sSwitchClosed;
    }
  else{
    sRelaySetting= sSwitchOpen;
    }
  sSetRelay(0, sRelaySetting);    //Set relay #0
  return;
  } //BLYNK_WRITE(TimerA_1V11)


BLYNK_WRITE(TimerB_1V12){
  int sSetting= param.asInt();
  int sRelaySetting;
  String szLogString= "Set TimerB_1V12 ";
  szLogString += sRelaySetting;
  sLogToBoth(szLogString);

  if (sSetting == 1){
    sRelaySetting= sSwitchClosed;
    }
  else{
    sRelaySetting= sSwitchOpen;
    }
  sSetRelay(0, sRelaySetting);
  return;
  } //BLYNK_WRITE(TimerB_1V12)


//WidgetLED oLED1(LED_1V13) is constructed earlier


BLYNK_WRITE(Switch_2V15){
  int sSetting= param.asInt();
  int sRelaySetting;
  String szLogString= "Set Switch2V15 ";
  szLogString += sRelaySetting;
  sLogToBoth(szLogString);

  if (sSetting == 1){
    sRelaySetting= sSwitchClosed;
  }
  else{
    sRelaySetting= sSwitchOpen;
  }
  sSetRelay(1, sRelaySetting);    //Set relay #1
  return;
} //BLYNK_WRITE(Switch_2V15)


BLYNK_WRITE(TimerA_2V16){
  int sSetting= param.asInt();
  int sRelaySetting;
  String szLogString= "Set TimerA_2V16 ";
  szLogString += sRelaySetting;
  sLogToBoth(szLogString);

  if (sSetting == 1){
    sRelaySetting= sSwitchClosed;
  }
  else{
    sRelaySetting= sSwitchOpen;
  }
  sSetRelay(1, sRelaySetting);
  return;
} //BLYNK_WRITE(TimerA_2V16)


BLYNK_WRITE(TimerB_2V17){
  int sSetting= param.asInt();
  int sRelaySetting;
  String szLogString= "Set TimerB_2V17 ";
  szLogString += sRelaySetting;
  sLogToBoth(szLogString);

  if (sSetting == 1){
    sRelaySetting= sSwitchClosed;
  }
  else{
    sRelaySetting= sSwitchOpen;
  }
  sSetRelay(1, sRelaySetting);
  return;
} //BLYNK_WRITE(TimerB_2V17)


//WidgetLED oLED1(LED_2V18) is constructed earlier

/*
BLYNK_WRITE(Terminal_V7)
{
  Serial << LOG0 << " BLYNK_WRITE(Terminal_V7): Received Parameter= " <<  param.asStr() << endl;
  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Marco") == param.asStr()) {
      oTerminal.println("You said: 'Marco'") ;
      oTerminal.println("I said: 'Polo'") ;
  } else {

  // Send it back
  oTerminal.print("You said:");
  oTerminal.write(param.getBuffer(), param.getLength());
  oTerminal.println();
  }

  // Ensure everything is sent
  oTerminal.flush();
  return;
} //BLYNK_WRITE(Terminal_V7)
*/
//Last line.
