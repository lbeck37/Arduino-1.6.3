//BeckTanks.h
#ifndef BECKTANKS_H
#define BECKTANKS_H
#include <BeckLib.h>

typedef enum {
  eGrey1= 1,
  eGrey2,
  eBlack,
  eLastTank
} Tank_t;

class BeckTanks {
protected:
  INT16           sNumTanks_          = (eLastTank - 1);
  const UINT32    ulTanksReadTime_    = 1000;   //Spacing between reading tanks

  uint32_t        ulNextTanksTime_    = 0;  //msec when the tanks will be read
  boolean         bGyroChanged_       = false;
public:
  double    adTankPercentFull_[eLastTank];

            BeckTanks     (void);
  void      Read          (void);
};  //BeckTanks
#endif  //BECKTANKS_H
