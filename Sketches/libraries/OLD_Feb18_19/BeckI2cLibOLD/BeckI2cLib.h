//BeckI2cLib.h
// ***OBSOLETE*** See Sketches\libraries\BeckI2C
#ifndef BECKI2CLIB_H
#define BECKI2CLIB_H
#include <BeckLib.h>
//#include <Adafruit_ADS1015.h>   //Just for the defines

extern INT16    sSetup_I2C();
extern INT16    sSetup_ADS1115();
extern double   dRead_ADS1115(INT16 sChannel, adsGain_t eGain);
extern INT16    sSetup_Gyro();
extern void     Read_Gyro();

#endif  //BECKI2CLIB_H