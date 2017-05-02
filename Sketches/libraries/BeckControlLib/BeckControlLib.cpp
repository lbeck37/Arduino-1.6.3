//BeckControlLib.cpp
#include <BeckControlLib.h>
#include <OneWire.h>

const int    sSwitchOpen           = 0;
const int    sSwitchClosed         = 1;
const int    sOff                  = 0;
const int    sOn                   = 1;
const int    sNotInit              = -3737;

const int    sNumSwitches_         = 4;
const long   sThermoTimesInRow     = 3;      //Max times temp is outside range before switch
const float  fMaxHeatRangeF        = 2.00;   //Temp above setpoint before heat is turned off

int          asSwitchState_[]       = {0, 0, 0, 0, 0};
int          asSwitchLastState_[]   = {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
int          sSetpointF_            = 37;
int          sThermoTimesCount_     = 0;      //Number of times temperature out of range
bool         bThermoOn_             = true;   //Whether thermostat is running.
bool         bHeatOn_               = false;  //If switch is on to turn on furnace.
//float        fThermoOffDegF_        = sSetpointF_ + fMaxHeatRangeF;
float        fThermoOffDegF_;

//const int    asSwitchPin[]         = {-1, 4, 5, 15, 16};      //0 is not a switch, switches are at 1,2,3,4
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 15};    //15 is 8266 TXD0 and broke I2c ????
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 2};     //2 is 8266 TXd1 and I2C stopped and blue LED on ESP12 module went on
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 0};     //0 is 8266 Flash pin and when used caused relay to go on and off
//const int    asSwitchPin[]         = {-1, 12, 13, 14, 16};    //16 is 8266 User and Wake and broke I2C
const int    asSwitchPin[]         = {-1, 12, 13, 14, 15};      //0 is not a switch, switches are at 1,2,3,4
const bool   abSwitchInverted[]    = {0, true, true, true, true};  //Opto-isolated relays close when pulled low.
const int    sThermoDummySwitch    = 0;  			//Thermostat Blynk LED lives with unused switch Relay #0.
const int    sHeatSwitchNum        = 2;      	//Was 1, switch number that turns heat on and off.

void HandleHeatSwitch(){
  String szLogString = "HandleHeatSwitch(): bHeatOn";
  LogToBoth(szLogString, bHeatOn_);
  //Serial << LOG0 << "HandleHeatSwitch(): bThermoOn, bFurnaceOn " << bThermoOn << ", " << bFurnaceOn << endl;
  //Make sure  switch state represents bHeatOn correctly.
  if (bHeatOn_){
    //Serial << LOG0 << "HandleFurnaceSwitch(): Set asSwitchState[sFurnaceSwitchNum] to sOn" << endl;
    asSwitchState_[sHeatSwitchNum]= sOn;
  } //if(bHeatOn)
  else{
    //Serial << LOG0 << "HandleFurnaceSwitch(): Set asSwitchState[sFurnaceSwitchNum] to sOff" << endl;
    asSwitchState_[sHeatSwitchNum]= sOff;
  } //if(bFurnaceOn)else
  SetSwitch(sHeatSwitchNum, asSwitchState_[sHeatSwitchNum]);
  return;
} //HandleHeatSwitch


void TurnHeatOn(bool bTurnOn){
  if (bTurnOn){
    String szLogString= "TurnHeatOn(): Heat turned ON";
    LogToBoth(szLogString);
    bHeatOn_= true;
    SetHeatSwitch(sSwitchClosed);
    sThermoTimesCount_= 0;
  } //if(bTurnOn)
  else{
    String szLogString= "TurnHeatOn(): Heat turned OFF";
    LogToBoth(szLogString);
    bHeatOn_= false;
    SetHeatSwitch(sSwitchOpen);
    sThermoTimesCount_= 0;
  } //if(bTurnOn)else
  return;
} //TurnHeatOn


void SetThermoState(int sSwitchState){
  asSwitchState_[sThermoDummySwitch]= sSwitchState;
  if (sSwitchState == sOn){
    bThermoOn_= true;
  } //if(sState==sOn)
  else{
    bThermoOn_= false;
    bHeatOn_= false;
    sThermoTimesCount_= 0;
    SetHeatSwitch(sSwitchOpen);
  } //if(sState==sOn)else
  return;
} //SetThermoState


void SetHeatSwitch(int sSwitchState){
  SetSwitch(sHeatSwitchNum, sSwitchState);
  return;
} //SetHeatSwitch


void SetSwitch(int sSwitch, int sSwitchState){
  int sSwitchPin= asSwitchPin[sSwitch];
  bool bPinSetting;
  asSwitchState_[sSwitch]= sSwitchState;
  //bDebugLog= bDebug;
  if (abSwitchInverted[sSwitch]){
    bPinSetting= !sSwitchState;
  } //if(abSwitchInverted[sSwitch])
  else{
    bPinSetting= sSwitchState;
  } //if(abSwitchInverted[sSwitch])else
  //Serial << LOG0 << "SetSwitch(): sSwitch, sSwitchState, sSwitchPin, bPinSetting " << sSwitch << ", " << sSwitchState << ", " << sSwitchPin << ", " << bPinSetting <<  endl;
  //Serial << LOG0 << "SetSwitch(): sSwitch, sSwitchState, sSwitchPin, bPinSetting" <<  endl;
  String szLogString= "SetSwitch:  ";
  szLogString += sSwitch;
  szLogString += ",";
  szLogString += sSwitchState;
  szLogString += ",";
  szLogString += sSwitchPin;
  szLogString += ",";
  szLogString += bPinSetting;
  //LogToBoth(szLogString);

  //Some switch positions don't have pins, just Blynk LEDs.
  if (sSwitchPin >= 0){
    digitalWrite(sSwitchPin, bPinSetting);
    //asSwitchState_[sSwitch]= sSwitchState;
  } //if(sSwitchPin>=0)
  //bDebugLog= true;
  //HandleBlynkLEDs();
  return;
} //SetSwitch
//Last line.
