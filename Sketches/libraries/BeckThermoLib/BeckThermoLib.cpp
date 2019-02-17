// BeckThermoLib.cpp 2/17/19a
#include <BeckThermoLib.h>
#include <BeckLogLib.h>
#include <BeckSwitchLib.h>
#include <DallasTemperature.h>
#include <OneWire.h>

float         fLastDegF             = 37.88;  //Last temperature reading.
int           sThermoTimesCount     = 0;      //Number of times temperature out of range
bool          bThermoOn             = true;   //Whether thermostat is running.
bool          bHeatOn               = false;  //If switch is on to turn on Heat.

float        _fSetpointF         = 70.0;
float        _fThermoOffDegF     = _fSetpointF + fMaxHeatRangeF;
float        _fMinSetpoint       = 32.0;
float        _fMaxSetpoint       = 75.0;

//Create OneWire instance and tell Dallas Temperature Library to use oneWire Library
OneWire             oOneWire(sOneWireGPIO);
DallasTemperature   oSensors(&oOneWire);


void HandleHeater(){
  String szLogString = "HandleHeater()";
  LogToSerial(szLogString);
  return;
} //HandleHeater


void HandleThermostat(){
  unsigned long   ulStartTime;
  ClearTaskTime2(&ulStartTime);
  //Only do something if the thermostat is turned on.
  if (bThermoOn){
    float fDegF= fGetDegF(true);
    CheckTaskTime2("HandleThermostat(): fGetDegF", &ulStartTime);
    if (bHeatOn){
      if (fDegF >= _fThermoOffDegF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnHeatOn(false);
          CheckTaskTime2("HandleThermostat(): TurnHeatOn(false)", &ulStartTime);
          sThermoTimesCount= 0;
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fDegF>=_fThermoOffDegF)
      else{
        sThermoTimesCount= 0;
      } //if(fDegF>=_fThermoOffDegF)else
    } //if(bHeatOn)
    else{
      if (fDegF <= _fSetpointF){
        if (++sThermoTimesCount >= sThermoTimesInRow){
          TurnHeatOn(true);
          CheckTaskTime2("HandleThermostat(): TurnHeatOn(true)", &ulStartTime);
          sThermoTimesCount= 0;
        } //if(sThermoTimesCount>=sThermoTimesInRow)
      } //if(fDegF<_fSetpointF)
      else{
        sThermoTimesCount= 0;
      } //if(fDegF<_fSetpointF)else
    } //if(bHeatOn)else
    LogThermostatData(fDegF);
    CheckTaskTime2("HandleThermostat(): LogThermostatData()", &ulStartTime);
  } //if(bThermoOn)
  else{
    String szLogString= " bThermoOn is false";
    LogToSerial(szLogString);
  }
  return;
} //HandleThermostat


void LogThermostatData(float fDegF){
  String szLogString= " " + String(bHeatOn) + String(sThermoTimesCount) + " " +
                String(fDegF) + " " + String(_fSetpointF) + " " + String(_fThermoOffDegF);
  LogToSerial(szLogString);
  return;
} //LogThermostatData
float fSetThermoSetpoint(int wSetpoint){
  float fSetpoint= round( ((float)wSetpoint / 255.0) * 100.0);
  fSetThermoSetpoint(fSetpoint);
  return fSetpoint;
} //fSetThermoSetpoint(int)


float fSetThermoSetpoint(float fSetpoint){
  float fLastSetpoint= _fSetpointF;
  if( (fSetpoint >= _fMinSetpoint) && (fSetpoint <= _fMaxSetpoint)){
    if(fSetpoint != _fSetpointF){
      _fSetpointF      = fSetpoint;
      _fThermoOffDegF  = _fSetpointF + fMaxHeatRangeF;
      Serial << LOG0 << "fSetThermoSetpoint(): Set _fSetpointF to " << _fSetpointF << endl;
    } //if(fSetpoint!=_fSetpointF)
  } //if((fSetpoint>=...
  if(_fSetpointF == fLastSetpoint){
    Serial << LOG0 << "fSetThermoSetpoint(): _fSetpointF remains at " << _fSetpointF << endl;
  } //if((_fSetpointF==fLastSetpoint)
  return _fSetpointF;
} //fSetThermoSetpoint(float)


void HandleHeatSwitch(){
  if (bHeatOn){
    SetSwitch(sHeatSwitchNum, sOn);
  } //if(bHeatOn)
  else{
    asSwitchState[sHeatSwitchNum]= sOff;
    SetSwitch(sHeatSwitchNum, sOff);
  } //if(bHeatOn)else
  return;
} //HandleHeatSwitch


float fGetDegF(bool bTakeReading){
  float fDegFReturn= 37.99;   //Value used for default in testing w/o reading sensor. fLastDegF
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


void TurnHeatOn(bool bTurnOn){
  if (bTurnOn){
    String szLogString= "TurnHeatOn(): ON";
    LogToSerial(szLogString);
    bHeatOn= true;
    SetHeatSwitch(sSwitchClosed);
    sThermoTimesCount= 0;
  } //if(bTurnOn)
  else{
    String szLogString= "TurnHeatOn(): OFF";
    LogToSerial(szLogString);
    bHeatOn= false;
    SetHeatSwitch(sSwitchOpen);
    sThermoTimesCount= 0;
  } //if(bTurnOn)else
  return;
} //TurnHeatOn

//Last line.
