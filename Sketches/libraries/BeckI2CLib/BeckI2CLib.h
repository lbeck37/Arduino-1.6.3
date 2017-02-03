//BeckI2cLib.h
#ifndef BECKI2CLIB_H
#define BECKI2CLIB_H
#include <BeckLib.h>

extern INT16 		sSetup_I2C();
extern INT16 		sSetup_ADS1115();
extern double 	dRead_ADS1115(INT16 sChannel);
extern INT16 		sSetup_Gyro();
extern void 		Read_Gyro();

#endif	//BECKI2CLIB_H