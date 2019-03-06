// BeckThermoLib.h 3/5/19a
#pragma once

const long          sThermoTimesInRow     = 3;      //Max times temp is outside range before switch
const float         fMaxHeatRangeF        = 0.10;   //Temp above setpoint before heat is turned off

extern float        fLastDegF;
extern float        _fSetpointF;
extern float        _fThermoOffDegF;
extern float        _fMinSetpoint;
extern float        _fMaxSetpoint;

void    HandleHeater          ();
void    HandleThermostat      ();
void    LogThermostatData     (float fDegF);
float   fSetThermoSetpoint    (unsigned char ucSetpoint);
float   fSetThermoSetpoint    (float fSetpoint);

void    HandleHeatSwitch      ();
float   fGetDegF              (bool bTakeReading);
float   fRound                (float fNum);
void    TurnHeatOn            (bool bTurnOn);

//Last line.
