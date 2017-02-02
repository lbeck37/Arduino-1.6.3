//BeckControlLib.h
#include <BeckLib.h>
#include <BeckI2cLib.h>
#include <DallasTemperature.h>

extern int          asSwitchState[];
extern int          asSwitchLastState[];
extern float        fLastDegF;
extern int          sSetpointF;
extern int          sThermoTimesCount;
extern bool         bThermoOn;
extern bool         bFurnaceOn;
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

extern DallasTemperature   oSensors;

//ESP32 AtoD Input Pins
extern const int	sNumTanks;
extern const int	sPinsPerTank;

extern const uint8_t ucGrey1PowerPin;
extern const uint8_t ucGrey1LevelPin;
extern const uint8_t ucBlackPowerPin;
extern const uint8_t ucBlackLevelPin;
extern const uint8_t ucGrey2PowerPin;
extern const uint8_t ucGrey2LevelPin;

//Function protos.
extern void SetupDevices();
//extern void ReadGyro();
extern float fReadAtoD(int sChannel);
extern void HandleFurnaceSwitch();
extern void TurnFurnaceOn(bool bTurnOn);
extern void SetThermoState(int sSwitchState);
extern void SetFurnaceSwitch(int sSwitchState);
extern void SetSwitch(int sSwitch, int sSwitchState);
extern float fGetDegF(bool bTakeReading);
//Last line.
