//BeckBlynk.cpp
#include <BeckBlynk.h>
#include <BeckControlLib.h>
//#include <BeckESP32AtoDLib.h>
#ifdef ESP8266
  #include <BlynkSimpleEsp8266.h>
#endif  //ESP8266
#ifdef ESP32
  #include <BlynkSimpleEsp32.h>
#endif  //ESP32

//Define Virtual Pin names (V0 - V127)
//DS18B20 OneWire temp sensors
#define ReadF_V40         V40
#define ReadF_V41         V41
#define ReadF_V42         V42
#define ReadF_V43         V43
#define ReadF_V44         V44
#define ReadF_V45         V45

#define ReadF_V0          V0
#define ReadF_V1          V1
#define SetSetpointF_V2   V2
#define GetSetpointF_V3   V3
#define ThermoSwitch_V4   V4
#define ThermoLED_V5      V5

#define AtoD_1V6          V6

#define Terminal_V7       V7
#define LCD_Line0_V8      V8
#define LCD_Line1_V9      V9

//Relay #1
#define Switch_1V10       V10
#define TimerA_1V11       V11
#define TimerB_1V12       V12
#define LED_1V13          V13

#define AtoD_2V14         V14

//Relay #2
#define Switch_2V15       V15
#define TimerA_2V16       V16
#define TimerB_2V17       V17
#define LED_2V18          V18

#define AtoD_3V19         V19

//Relay #3
#define Switch_3V20       V20
#define TimerA_3V21       V21
#define TimerB_3V22       V22
#define LED_3V23          V23

#define AtoD_4V24         V24

//Relay #4
#define Switch_4V25       V25
#define TimerA_4V26       V26
#define TimerB_4V27       V27
#define LED_4V28          V28

#define Unassigned_V29    V29
#define FlowLED_V30    		V30
#define Unassigned_V31    V31

//Blynk Terminal
WidgetTerminal      oTerminal(Terminal_V7);

WidgetLCD           LCDWidget(1);

//LED for thermostat state has no actual switch but it will live as unused switch #0.
WidgetLED           oLED0(ThermoLED_V5);
WidgetLED           oLED1(LED_1V13);
WidgetLED           oLED2(LED_2V18);
WidgetLED           oLED3(LED_3V23);
WidgetLED           oLED4(LED_4V28);
WidgetLED           oLED5(FlowLED_V30);

BeckBlynk*      pBeckBlynk_;
BeckAtoD*       pBeckAtoD_;

//class BeckBlynk
BeckBlynk::BeckBlynk(const char acBlynkAuthToken[], BeckAtoD* pBeckAtoD) {
  String szLogString="BeckBlynk Constructor: Begin";
  LogToSerial(szLogString);
  pBeckBlynk_= this;
  pBeckAtoD_= pBeckAtoD;
  Blynk.config(acBlynkAuthToken);
  return;
} //Constructor


void BeckBlynk::Run() {
  Blynk.run();
  return;
} //Run

/*
double BeckBlynk::dReadAtoD(INT16 sChannel) {
  return(pBeckAtoD_->dRead(sChannel, GAIN_ONE));
} //dReadAtoD
*/

//Local function protos.
void WriteTerminalLine(String szString);
void WriteTerminalString(String szString);

//Functions
void BlynkLogLine(String szString){
  String szTermString= szLogLineHeader(lLineCount);
  szTermString += szString;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:empty


void BlynkLogLine(String szString, String szLogValue){
  String szTermString= szLogLineHeader(lLineCount);
  szTermString += szString;
  szTermString +=  " ";
  szTermString +=  szLogValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:String


void BlynkLogLine(String szString, int sValue){
  String szTermString= szLogLineHeader(lLineCount);
  szTermString += szString;
  szTermString +=  " ";
  szTermString +=  sValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:int


void BlynkLogLine(String szString, float fValue) {
  String szTermString= szLogLineHeader(lLineCount);
  szTermString += szString;
  szTermString +=  fValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:float


void BlynkLogLine(String szString, double dValue) {
  String szTermString= szLogLineHeader(lLineCount);
  szTermString += szString;
  szTermString +=  dValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:double


void HandleBlynkLEDs(){
  String szLogString = "HandleBlynkLEDs()";
  LogToBoth(szLogString);
  //DebugHandleBlynkLEDs();
  //Only send data back to Blynk if state of LED has changed.
  //static int asSwitchLastState[]= {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
  //bDebugLog= bDebug;  //If bDebug is OfF then we turn logging off for this routine, resetting at end.
  for (int sSwitch= 0; sSwitch <= sNumSwitches_; sSwitch++){
    if (true || (abSwitchState_[sSwitch] != abSwitchLastState_[sSwitch])){
      abSwitchLastState_[sSwitch]= abSwitchState_[sSwitch];
      switch (sSwitch){
        case 0:
          if (abSwitchState_[sSwitch]){
            szLogString = "Switch ON LED";
            oLED0.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED0.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 1:
          if (abSwitchState_[sSwitch]){
            szLogString = "Switch ON LED";
            oLED1.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch ON LED";
            oLED1.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 2:
          if (abSwitchState_[sSwitch]){
            szLogString = "Switch ON LED";
            oLED2.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED2.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 3:
          if (abSwitchState_[sSwitch]){
            szLogString = "Switch ON LED";
            oLED3.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED3.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 4:
          if (abSwitchState_[sSwitch]){
            szLogString = "Switch ON LED";
            oLED4.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED4.off();
          } //if(asSwitchState[sSwitch])else
          break;
        default:
          break;
      } //switch
      LogToBoth(szLogString, sSwitch);
    } //if(asSwitchState[sSwitch]!=asSwitchLastState[sSwitch])
  } //for

  bFlowLastState_= bFlowState_;
	if (bFlowState_){
		szLogString = "Switch ON LED Flow";
    LogToBoth(szLogString);
		oLED5.on();
	} //if(asSwitchState[sSwitch])
	else{
		szLogString = "Switch OFF LED Flow";
    LogToBoth(szLogString);
		oLED5.off();
	} //if(asSwitchState[sSwitch])else

  bDebugLog= true;
  return;
} //HandleBlynkLEDs


// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
//int WriteTerminalLine(char *szString){
void WriteTerminalLine(String szString){
  if (bDebugLog){
    oTerminal.println(szString) ;
    oTerminal.flush();          // Ensure everything is sent
  } //if(bDebugLog)
  return;
} //WriteTerminalLine


void WriteTerminalString(String szString){
  if (bDebugLog){
    oTerminal.print(szString);
    oTerminal.print(" ");       //Send training space
    oTerminal.flush();          // Ensure everything is sent
  } //if(bDebugLog)
  return;
} //WriteTerminalString

void SendIntToBlynk(int sVirtualPin, int sValue){
  String szString= " SendIntToBlynk: ";
  BlynkLogLine(szString, sValue);
  Blynk.virtualWrite(sVirtualPin, sValue);
  return;
} //SendIntToBlynk


//BLYNK_READ() functions are called by the Blynk app on the phone (at a 1 second interval)
//and returns the value or state of some variable.
//BLYNK_WRITE() functions are called by the Blynk app on the phone
//and pass a variable in the "param" object.
BLYNK_READ(ReadF_V40){
  float fDegF= pBeckOneWire->fGetDegF(eVP40);
  String szLogString= "Read ReadF_V40 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V40, fDegF);
} //BLYNK_READ(ReadF_V41)


BLYNK_READ(ReadF_V41){
  float fDegF= pBeckOneWire->fGetDegF(eVP41);
  String szLogString= "Read ReadF_V41 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V41, fDegF);
} //BLYNK_READ(ReadF_V41)


BLYNK_READ(ReadF_V42){
  float fDegF= pBeckOneWire->fGetDegF(eVP42);
  String szLogString= "Read ReadF_V42 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V42, fDegF);
} //BLYNK_READ(ReadF_V42)


BLYNK_READ(ReadF_V43){
  float fDegF= pBeckOneWire->fGetDegF(eVP43);
  String szLogString= "Read ReadF_V43 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V43, fDegF);
} //BLYNK_READ(ReadF_V43)


BLYNK_READ(ReadF_V44){
  float fDegF= pBeckOneWire->fGetDegF(eVP44);
  String szLogString= "Read ReadF_V44 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V44, fDegF);
} //BLYNK_READ(ReadF_V44)


BLYNK_READ(ReadF_V45){
  float fDegF= pBeckOneWire->fGetDegF(eVP45);
  String szLogString= "Read ReadF_V45 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V45, fDegF);
} //BLYNK_READ(ReadF_V45)


BLYNK_READ(ReadF_V0){
  bool bTakeReading= true;
  float fDegF= fGetDegF(bTakeReading);
  String szLogString= "Read ReadF_V0 ";
  LogToBoth(szLogString, fDegF);

  //Blynk.virtualWrite(ReadF_V0, fRound(fDegF));
  Blynk.virtualWrite(ReadF_V0, fDegF);
} //BLYNK_READ(ReadF_V0)


BLYNK_READ(ReadF_V1){
  bool bTakeReading= false;
  float fDegF= fGetDegF(bTakeReading);
  String szLogString= "Read ReadF_V1 ";
  LogToBoth(szLogString, fDegF);

  Blynk.virtualWrite(ReadF_V1, fDegF);
} //BLYNK_READ(ReadF_V1)


BLYNK_WRITE(SetSetpointF_V2){
  int sSetpointParam= param.asInt();
  sSetpointF_= sSetpointParam;
  fThermoOffDegF_= sSetpointF_ + fMaxHeatRangeF;
  String szLogString= "SetSetpointF_V2 ";
  LogToBoth(szLogString, sSetpointF_);

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  SendIntToBlynk(GetSetpointF_V3, sSetpointF_);
  return;
} //BLYNK_WRITE(Switch_2V15)


BLYNK_READ(GetSetpointF_V3){
  int sReturnF= sSetpointF_;
  String szLogString= "GetSetpointF_V3 ";
  LogToBoth(szLogString, sSetpointF_);

  Blynk.virtualWrite(GetSetpointF_V3, sReturnF);
} //BLYNK_READ(GetSetpointF_V3)


BLYNK_WRITE(ThermoSwitch_V4){
  //Turn thermostat on and off.
  int sParam= param.asInt();
  String szLogString= "ThermoSwitch_V4 ";
  LogToBoth(szLogString, sParam);
  SetThermoState(sParam);
  //HandleHeatSwitch();
  HandleBlynkLEDs();
  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  //SendIntToBlynk(GetSetpointF_V3, sSetpointF_);
  return;
} //BLYNK_WRITE(ThermoSwitch_V4)

//WidgetLED oLED0(ThermoLED_V5) is constructed earlier

BLYNK_READ(AtoD_1V6){
  //double dVolts= pBeckBlynk_->dReadAtoD(1);
  double dVolts= pBeckAtoD_->dRead(1, GAIN_ONE);
  String szLogString= "Read AtoD_1V6 ";
  LogToBoth(szLogString, dVolts);
  Blynk.virtualWrite(AtoD_1V6, dVolts);
} //BLYNK_READ(AtoD_1V6)


//Handler callback function called when Button set as a Switch is pressed.
//Light around button is lit when 1 is passed as parameter, unlit when 0 is passed.
//Opto-isolated relay is inverse logic, pulling input pin low cn relay.
//Relay #0 is associated with Blynk virtual pins 10, 11, 12, 13
//Relay #1 is associated with Blynk virtual pins 15, 16, 17, 18
//	Each "Relay" has a switch, 2 timers, and an LED. Timers never worked
BLYNK_WRITE(Switch_1V10){
  //Virtual pin 10, 15, 20 and 25 control Switches 1, 2, 3 and 4.
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();

  String szLogString= "Set Switch_1V10 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
/*
  //Test writing to LCD
  LCDWidget.clear();
  int sCharPos= 0;   //Position 0-15
  int sLineNum= 0;   //Line 0-1
  LCDWidget.print(0, 0, "Relay #0 set to: ");
*/
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  HandleBlynkLEDs();
  return;
} //BLYNK_WRITE(Switch_1V10)


BLYNK_WRITE(TimerA_1V11){
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_1V11 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
    }
  else{
    sSwitchSetting= sSwitchOpen;
    }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
  } //BLYNK_WRITE(TimerA_1V11)


BLYNK_WRITE(TimerB_1V12){
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_1V12 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
    }
  else{
    sSwitchSetting= sSwitchOpen;
    }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
  } //BLYNK_WRITE(TimerB_1V12)


//WidgetLED oLED1(LED_1V13) is constructed earlier


BLYNK_READ(AtoD_2V14){
  //double dVolts= pBeckBlynk_->dReadAtoD(2);
  double dVolts= pBeckAtoD_->dRead(2, GAIN_ONE);
  String szLogString= "Read AtoD_2V14 ";
  LogToBoth(szLogString, dVolts);
  Blynk.virtualWrite(AtoD_2V14, dVolts);
} //BLYNK_READ(AtoD_2V14)


BLYNK_WRITE(Switch_2V15){
  int sSwitchNumber= 2;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set Switch2V15 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_2V15)


BLYNK_WRITE(TimerA_2V16){
  int sSwitchNumber= 2;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_2V16 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerA_2V16)


BLYNK_WRITE(TimerB_2V17){
  int sSwitchNumber= 2;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_2V17 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerB_2V17)


//WidgetLED oLED1(LED_2V18) is constructed earlier


BLYNK_READ(AtoD_3V19){
//  double dVolts= pBeckBlynk_->dReadAtoD(3);
  double dVolts= pBeckAtoD_->dRead(3, GAIN_ONE);
  String szLogString= "Read AtoD_3V19 ";
  LogToBoth(szLogString, dVolts);
  Blynk.virtualWrite(AtoD_3V19, dVolts);
} //BLYNK_READ(AtoD_3V19)


BLYNK_WRITE(Switch_3V20){
  int sSwitchNumber= 3;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set Switch_3V20 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_3V20)


BLYNK_WRITE(TimerA_3V21){
  int sSwitchNumber= 3;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_3V21 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerA_3V21)


BLYNK_WRITE(TimerB_3V22){
  int sSwitchNumber= 3;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_3V22 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerB_3V22)


//WidgetLED oLED1(LED_3V23) is constructed earlier


BLYNK_READ(AtoD_4V24){
  //double dVolts= dRead_AtoD(4);
//  double dVolts= pBeckBlynk_->dReadAtoD(4);
  double dVolts= pBeckAtoD_->dRead(4, GAIN_ONE);
  String szLogString= "Read AtoD_4V24 ";
  LogToBoth(szLogString, dVolts);
  Blynk.virtualWrite(AtoD_4V24, dVolts);
} //BLYNK_READ(AtoD_4V24)


BLYNK_WRITE(Switch_4V25){
  int sSwitchNumber= 4;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set Switch_4V25 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(Switch_4V25)


BLYNK_WRITE(TimerA_4V26){
  int sSwitchNumber= 4;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerA_4V26 ";
  szLogString += sSetting;
  LogToBoth(szLogString);
  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerA_4V26)


BLYNK_WRITE(TimerB_4V27){
  int sSwitchNumber= 4;
  int sSwitchSetting;
  int sSetting= param.asInt();
  String szLogString= "Set TimerB_4V27 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
  return;
} //BLYNK_WRITE(TimerB_4V27)

//Last line.
