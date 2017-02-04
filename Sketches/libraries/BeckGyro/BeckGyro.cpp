//BeckGyro.cpp
#include <BeckGyro.h>
//#include <BeckI2cLib.h>
#include <Wire.h>

//BeckAtoD class methods
BeckGyro::BeckGyro(void) {
	Wire.begin();
	return;
}	//Constructor


void BeckGyro::Read(void) {

}	//dRead


void BeckGyro::Setup(void) {
  //Initialize the data array.
  for (int sDataType= eAccel; sDataType < sNumGyroSensors_; sDataType++) {
     for (int sAxis= eXAxis; sAxis < sNumAxis_; sAxis++) {
        asGyro_[sDataType][sAxis]= 0;
     }  //for sDataType
  }  //for sAxis

	return;
}	//Setup


//Last line.
