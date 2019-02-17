// BeckThermoLib.h 2/16/19a

const float         fMaxHeatRangeF      = 0.10;   //Temp above setpoint before heat is turned off
extern float        _fSetpointF;
extern float        _fThermoOffDegF;
extern float        _fMinSetpoint;
extern float        _fMaxSetpoint;


float   fSetThermoSetpoint    (int wSetpoint);
float   fSetThermoSetpoint    (float fSetpoint);

//Last line.
