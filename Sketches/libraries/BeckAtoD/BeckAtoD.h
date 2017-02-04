//BeckAtoD.h
#ifndef BECKATOD_H
#define BECKATOD_H
#include <BeckLib.h>

typedef enum {
	eADS1115= 1,
	eESP32_AtoD
} AtoD_t;

class BeckAtoD {
protected:
	AtoD_t 			eType_;
public:
						BeckAtoD		(AtoD_t eType);
	double 		dRead				(INT16 sChan, adsGain_t eGain);
	void			SetAddress	(UINT8 ucAddress);
};	//BeckAtoD
#endif	//BECKATOD_H