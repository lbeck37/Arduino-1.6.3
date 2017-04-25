//BeckOneWire.h
#ifndef BECKONEWIRE_H
#define BECKONEWIRE_H
#include <BeckLib.h>
#include <DallasTemperature.h>

extern DallasTemperature   oSensors;

extern float    fGetDegF(bool bTakeReading);

class BeckOneWire {
protected:

public:
	BeckOneWire(DeviceAddress aucDeviceAddress);

private:
	DeviceAddress		aucDeviceAddress_;

};	//BeckOneWire

#endif  //BECKONEWIRE_H
