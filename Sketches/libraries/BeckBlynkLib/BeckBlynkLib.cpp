//BeckBlynkLib.cpp

#include <BeckBlynkLib.h>
#include <BeckControlLib.h>
#include <BeckESP32AtoDLib.h>
#ifdef ESP8266
  #include <BlynkSimpleEsp8266.h>
#endif  //ESP8266
#ifdef ESP32
  #include <BlynkSimpleEsp32.h>
#endif	//ESP32

//Define Virtual Pin names
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
#define Unassigned_V30    V30
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

//Local function protos.
void WriteTerminalLine(String szString);
void WriteTerminalString(String szString);


//Functions
void RunBlynk() {
   Blynk.run();
return;
}	//RunBlynk


void ConfigBlynk(const char acBlynkAuthToken[]) {
   Blynk.config(acBlynkAuthToken);
return;
}	//ConfigBlynk


void ConfigBlynk(const char acBlynkAuthToken[], IPAddress oIPAddress) {
	   Blynk.config(acBlynkAuthToken, oIPAddress);
return;
}	//ConfigBlynk


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


void BlynkLogLine(String szString, float fValue){
  String szTermString= szLogLineHeader(lLineCount);
  szTermString += szString;
  szTermString +=  fValue;
  WriteTerminalLine(szTermString);
  return;
} //BlynkLogLine:float


void HandleBlynkLEDs(){
  String szLogString = "HandleBlynkLEDs()";
  LogToBoth(szLogString);
  //DebugHandleBlynkLEDs();
  //Only send data back to Blynk if state of LED has changed.
  //static int asSwitchLastState[]= {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
  //bDebugLog= bDebug;  //If bDebug is OfF then we turn logging off for this routine, resetting at end.
  for (int sSwitch= 0; sSwitch <= sNumSwitches; sSwitch++){
    if (true || (asSwitchState[sSwitch] != asSwitchLastState[sSwitch])){
      asSwitchLastState[sSwitch]= asSwitchState[sSwitch];
      switch (sSwitch){
        case 0:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED0.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED0.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 1:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED1.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch ON LED";
            oLED1.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 2:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED2.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED2.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 3:
          if (asSwitchState[sSwitch]){
            szLogString = "Switch ON LED";
            oLED3.on();
          } //if(asSwitchState[sSwitch])
          else{
            szLogString = "Switch OFF LED";
            oLED3.off();
          } //if(asSwitchState[sSwitch])else
          break;
        case 4:
          if (asSwitchState[sSwitch]){
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
      //LogToBoth(szLogString, sSwitch);
    } //if(asSwitchState[sSwitch]!=asSwitchLastState[sSwitch])
  } //for
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
  sSetpointF= sSetpointParam;
  fThermoOffDegF= sSetpointF + fMaxHeatRangeF;
  String szLogString= "SetSetpointF_V2 ";
  LogToBoth(szLogString, sSetpointF);

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  SendIntToBlynk(GetSetpointF_V3, sSetpointF);
  return;
} //BLYNK_WRITE(Switch_2V15)


BLYNK_READ(GetSetpointF_V3){
  int sReturnF= sSetpointF;
  String szLogString= "GetSetpointF_V3 ";
  LogToBoth(szLogString, sSetpointF);

  Blynk.virtualWrite(GetSetpointF_V3, sReturnF);
} //BLYNK_READ(GetSetpointF_V3)


BLYNK_WRITE(ThermoSwitch_V4){
  //Turn thermostat on and off.
  int sParam= param.asInt();
  String szLogString= "ThermoSwitch_V4 ";
  LogToBoth(szLogString, sParam);
  SetThermoState(sParam);
  HandleFurnaceSwitch();

  //Send set point back to Value box set with PUSH from GetSetpointF_V3.
  SendIntToBlynk(GetSetpointF_V3, sSetpointF);
  //HandleBlynkLEDs();
  return;
} //BLYNK_WRITE(ThermoSwitch_V4)

//WidgetLED oLED0(ThermoLED_V5) is constructed earlier

BLYNK_READ(AtoD_1V6){
  float fVolts= fReadAtoD(ucGrey1LevelPin);
  String szLogString= "Read AtoD_1V6 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_1V6, fVolts);
} //BLYNK_READ(AtoD_1V6)


//Handler callback function called when Button set as a Switch is pressed.
//Light around button is lit when 1 is passed as parameter, unlit when 0 is passed.
//Opto-isolated relay is inverse logic, pulling input pin low cn relay.
//Relay #0 is connected to Blynk virtual pins 10, 11, 12
//Relay #1 is connected to Blynk virtual pins 20, 21, 22
BLYNK_WRITE(Switch_1V10){
  //Virtual pin 10, 15, 20 and 25 control Switches 1, 2, 3 and 4.
  int sSwitchNumber= 1;
  int sSwitchSetting;
  int sSetting= param.asInt();

  String szLogString= "Set Switch_1V10 ";
  szLogString += sSetting;
  LogToBoth(szLogString);

  //Test writing to LCD
  LCDWidget.clear();
  int sCharPos= 0;   //Position 0-15
  int sLineNum= 0;   //Line 0-1
  LCDWidget.print(0, 0, "Relay #0 set to: ");

  if (sSetting == 1){
    sSwitchSetting= sSwitchClosed;
  }
  else{
    sSwitchSetting= sSwitchOpen;
  }
  SetSwitch(sSwitchNumber, sSwitchSetting);
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
  float fVolts= fReadAtoD(ucBlackLevelPin);
  String szLogString= "Read AtoD_2V14 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_2V14, fVolts);
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
  float fVolts= fReadAtoD(ucGrey2LevelPin);
  String szLogString= "Read AtoD_3V19 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_3V19, fVolts);
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
  float fVolts= fReadAtoD(3);
  String szLogString= "Read AtoD_4V24 ";
  LogToBoth(szLogString, fVolts);
  Blynk.virtualWrite(AtoD_4V24, fVolts);
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
