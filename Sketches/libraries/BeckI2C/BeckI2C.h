//BeckI2C.h
#ifndef BECKI2C_H
#define BECKI2C_H
#include <BeckLib.h>

class BeckI2C {
protected:
  BeckI2C*    pBeckI2C_;
  INT16       sSetup_I2C          (void);

public:
              BeckI2C             (INT16 sDummy);
  INT16       sReadTwoBytes       (void) ;
  void        WriteI2cRegister    (UINT8 ucI2cAddress, UINT8 ucRegister, UINT16 usValue);
  INT16       ReadI2cRegister     (UINT8 ucI2cAddress, UINT8 ucRegister);
};  //BeckI2C
#endif  //BECKI2C_H
