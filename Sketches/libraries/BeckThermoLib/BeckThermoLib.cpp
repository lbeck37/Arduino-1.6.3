// BeckThermoLib.cpp 2/16/19a
#include <BeckThermoLib.h>
#include <BeckLogLib.h>

//const float  fMaxHeatRangeF      = 0.10;   //Temp above setpoint before heat is turned off
float        _fSetpointF         = 70.0;
float        _fThermoOffDegF     = _fSetpointF + fMaxHeatRangeF;
float          _fMinSetpoint         = 32.0;
float          _fMaxSetpoint         = 75.0;

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
//Last line.
