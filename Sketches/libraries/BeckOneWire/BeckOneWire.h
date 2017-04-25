//BeckOneWire.h
#ifndef BECKONEWIRE_H
#define BECKONEWIRE_H
#include <BeckLib.h>
#include <DallasTemperature.h>

extern DallasTemperature   oSensors;

extern float    fGetDegF(bool bTakeReading);

#endif  //BECKONEWIRE_H
