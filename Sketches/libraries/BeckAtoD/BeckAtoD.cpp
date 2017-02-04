//BeckAtoD.cpp
#include <BeckAtoD.h>
#include <BeckI2cLib.h>

//BeckAtoD class methods
BeckAtoD::BeckAtoD(AtoD_t eType) {
	eType_= eType;
	return;
}	//Constructor


double BeckAtoD::dRead(INT16 sChan, adsGain_t eGain) {
	return(dRead_ADS1115(sChan, eGain));
}	//dRead


void BeckAtoD::SetAddress(UINT8 ucAddress) {
	return;
}	//SetAddress
//Last line.