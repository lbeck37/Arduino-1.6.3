//BeckESPAtoDLib.h
#include <BeckLib.h>

//ESP32 AtoD Input Pins
extern const int  sNumTanks;
extern const int  sPinsPerTank;

extern const uint8_t ucGrey1PowerPin;
extern const uint8_t ucGrey1LevelPin;
extern const uint8_t ucBlackPowerPin;
extern const uint8_t ucBlackLevelPin;
extern const uint8_t ucGrey2PowerPin;
extern const uint8_t ucGrey2LevelPin;

/*
extern const int    sNumTanks;
extern const int    sPinsPerTank;
extern const uint8_t  ucTankPin[sNumTanks][sPinsPerTank];
*/

extern float fReadESPAtoD(int sChannel);

//Last line.
