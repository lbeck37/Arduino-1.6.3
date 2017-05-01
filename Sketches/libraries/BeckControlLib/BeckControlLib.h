//BeckControlLib.h
#ifndef BECKCONTROLLIB_H
#define BECKCONTROLLIB_H
#include <BeckLib.h>
#include <BeckOneWire.h>

extern int          asSwitchState_[];
extern int          asSwitchLastState_[];
extern float        fLastDegF_;
extern int          sSetpointF_;
extern int          sThermoTimesCount_;
extern bool         bThermoOn_;
extern bool         bHeatOn_;
extern float        fThermoOffDegF;
extern const long   sThermoTimesInRow;
extern const float  fMaxHeatRangeF;

extern const int    sSwitchOpen;
extern const int    sSwitchClosed;
extern const int    sOff;
extern const int    sOn;
extern const int    sNotInit;

extern const int    sNumSwitches;
extern const int    asSwitchPin[];    //0 is not a switch, switches are at 1,2,3,4

//ESP32 AtoD Input Pins
extern const int  sNumTanks;
extern const int  sPinsPerTank;

extern const uint8_t ucGrey1PowerPin;
extern const uint8_t ucGrey1LevelPin;
extern const uint8_t ucBlackPowerPin;
extern const uint8_t ucBlackLevelPin;
extern const uint8_t ucGrey2PowerPin;
extern const uint8_t ucGrey2LevelPin;

//Function protos.
extern void     HandleHeatSwitch();
extern void     TurnHeatOn(bool bTurnOn);
extern void     SetThermoState(int sSwitchState);
extern void     SetHeatSwitch(int sSwitchState);
extern void     SetSwitch(int sSwitch, int sSwitchState);

#endif  //BECKCONTROLLIB_H
