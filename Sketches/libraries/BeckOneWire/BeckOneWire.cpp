//BeckOneWire.cpp
#include <BeckOneWire.h>
#include <OneWire.h>

float        fLastDegF             = 37.37;  //Last temperature reading.
const int    sOneWirePin           = 12;  //Dallas DS18B20 Temperature Sensor

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
