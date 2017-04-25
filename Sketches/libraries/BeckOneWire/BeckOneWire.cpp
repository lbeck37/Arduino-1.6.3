//BeckOneWire.cpp
#include <BeckOneWire.h>
#include <OneWire.h>

float        					fLastDegF             = 37.37;  //Last temperature reading.
const int    					sOneWirePin           = 12;  //Dallas DS18B20 Temperature Sensor

//(4) Bare DS18B20 temperature sensors
const DeviceAddress		acSensorA							= {0x28, 0xFF, 0xBA, 0xB4, 0xA0, 0x16, 0x05, 0xAA};
const DeviceAddress		acSensorB							= {0x28, 0xFF, 0x80, 0xF8, 0xA0, 0x16, 0x03, 0xBC};
const DeviceAddress		acSensorC							= {0x28, 0xFF, 0x37, 0xC4, 0xA0, 0x16, 0x04, 0x7B};
const DeviceAddress		acSensorD							= {0x28, 0xFF, 0x03, 0xAF, 0xA0, 0x16, 0x05, 0xBA};

OneWire      oOneWire(sOneWirePin);
//Tell Dallas Temperature Library to use oneWire Library
DallasTemperature   oSensors(&oOneWire);

float fGetDegF(bool bTakeReading){
  float fDegFReturn= 37.37;   //Value used for default in testing w/o reading sensor. fLastDegF
#ifdef ESP8266
  if (bTakeReading){
    oSensors.requestTemperatures(); // Send the command to get temperatures
    fDegFReturn= oSensors.getTempFByIndex(0);
    fLastDegF= fDegFReturn;
  } //if(bTakeReading)
  else{
    fDegFReturn= fLastDegF;
  } //if(bTakeReading)else
#endif
  return fDegFReturn;
}  //fGetDegF

BeckOneWire::BeckOneWire(DeviceAddress aucDeviceAddress) {
	//Copy the 8 address bytes
	for (int sByte= 0; sByte < 8; sByte++) {
			aucDeviceAddress_[sByte]= aucDeviceAddress[sByte];
	}
	return;
}	//Constructor
//Last line.
