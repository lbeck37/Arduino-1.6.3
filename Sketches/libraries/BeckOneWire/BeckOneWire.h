//BeckOneWire.h
#ifndef BECKONEWIRE_H
#define BECKONEWIRE_H
#include <BeckLib.h>
#include <DallasTemperature.h>

extern DallasTemperature   oSensors;

extern float    fGetDegF(bool bTakeReading);

extern const DeviceAddress		acSensorA;
extern const DeviceAddress		acSensorB;
extern const DeviceAddress		acSensorC;
extern const DeviceAddress		acSensorD;

extern const DeviceAddress		acOneWireV40= acSensorB;
extern const DeviceAddress		acOneWireV41= acSensorC;
extern const DeviceAddress		acOneWireV42= acSensorD;

class BeckOneWire {
protected:

public:
	BeckOneWire(DeviceAddress aucDeviceAddress);
	float 	fGetDegF(bool bTakeReading);

private:
	DeviceAddress		aucDeviceAddress_;
	UINT8						ucResolution_				= 12;

};	//BeckOneWire

extern BeckOneWire*		pOneWireV40;
extern BeckOneWire*		pOneWireV41;
extern BeckOneWire*		pOneWireV42;
#endif  //BECKONEWIRE_H
