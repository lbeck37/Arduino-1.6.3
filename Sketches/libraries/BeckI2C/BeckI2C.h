//BeckI2C.h
#ifndef BECKI2C_H
#define BECKI2C_H
#include <BeckLib.h>
#include <map>

using namespace std;

typedef enum {
  eGyro= 1,
  eAtoD,
  eDisplay,
  eLastI2cDevice
} I2cDevice_t;

class BeckI2C {
protected:
  BeckI2C*    										pBeckI2C_;
  bool        										bSkipTestI2c_   = true;
  bool														bDevicePresent_[eLastI2cDevice];
  std::map<UINT8, I2cDevice_t>		mI2cAddresses {
  	{0x48, eAtoD},
		{0x68, eGyro},
		{0x3C, eDisplay}
  };
public:
              BeckI2C             (INT16 sDummy);
  bool				bDevicePresent			(I2cDevice_t eDevice);
  INT16       sReadTwoBytes       (void) ;
  void        WriteI2cRegister    (UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue);
  INT16       ReadI2cRegister     (UINT8 ucI2cAddress, UINT8 ucRegister);
  void        TestI2C             (void);
  UINT8       ucTestI2C           (UINT8 ucAddress);
};  //BeckI2C

class I2cDevice {
protected:
	I2cDevice_t		eDeviceType;
	String				szName;

							I2cDevice(void);
};
#endif  //BECKI2C_H
