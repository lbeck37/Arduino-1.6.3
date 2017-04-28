//BeckOneWire.h
#ifndef BECKONEWIRE_H
#define BECKONEWIRE_H
#include <BeckLib.h>
#include <DallasTemperature.h>

extern DallasTemperature   oSensors;

extern float    fGetDegF(bool bTakeReading);

/*
extern const DeviceAddress		acOneWireV40;
extern const DeviceAddress		acOneWireV41;
extern const DeviceAddress		acOneWireV42;
extern const DeviceAddress		acOneWireV43;
*/

/*
extern const DeviceAddress		acOneWireV40= acSensorB;
extern const DeviceAddress		acOneWireV41= acSensorC;
extern const DeviceAddress		acOneWireV42= acSensorD;
*/

class BeckOneWire {
protected:

public:
	BeckOneWire(INT16 sOneWirePin);

private:
};	//BeckOneWire


class BeckOneWireSensor {
protected:

public:
	BeckOneWireSensor(DeviceAddress aucDeviceAddress);
	float 	fGetDegF(bool bTakeReading);

private:
	DeviceAddress		aucDeviceAddress_;
	UINT8						ucResolution_				= 12;
};	//BeckOneWireSensor

/*
extern BeckOneWire*		pOneWireV40;
extern BeckOneWire*		pOneWireV41;
extern BeckOneWire*		pOneWireV42;
*/
extern BeckOneWire*		pBeckOneWire;
#endif  //BECKONEWIRE_H
