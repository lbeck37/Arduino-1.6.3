//BeckAtoD.h
#ifndef BECKATOD_H
#define BECKATOD_H
#include <BeckLib.h>
#include <BeckI2C.h>

typedef enum {
	eADS1115= 1,
	eESP32_AtoD
} AtoD_t;


class BeckAtoD {
protected:
	AtoD_t 				eType_;
	BeckI2C*			pBeckI2C_;
	const UINT8		ucADS1115_Address	 = 0x48;
	const UINT16 	usDefaultSingleChanReadConfig_=
																		ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
																		ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
																		ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
																		ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
																		ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
																		ADS1015_REG_CONFIG_MODE_SINGLE  | // Single-shot mode (default)
																		ADS1015_REG_CONFIG_OS_SINGLE;     // Single-shot start conversion
	double 		dRead_ADS1115		(INT16 sChannel, adsGain_t eGain);
public:
						BeckAtoD				(BeckI2C* pBeckI2C, AtoD_t eType);
	double 		dRead						(INT16 sChan, adsGain_t eGain);
	void			SetAddress			(UINT8 ucAddress);
};	//BeckAtoD
#endif	//BECKATOD_H