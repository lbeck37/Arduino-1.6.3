//BeckGyro.h
#ifndef BECKGYRO_H
#define BECKGYRO_H
#include <BeckLib.h>

typedef enum {
  eXAxis= 1,
  eYAxis,
  eZAxis,
  eLastAxis
} Axis_t;

typedef enum {
  eAccel= 1,
  eRotation,
  eTemperature,
  eLastSensor
} GyroSensor;

class BeckGyro {
protected:
  INT16     sNumAxis_           = (eLastAxis   - 1);
  INT16     sNumGyroSensors_    = (eLastSensor - 1);
public:
  INT16     asGyro_[eLastSensor - 1][eLastAxis - 1];

            BeckGyro    (void);
  void      Read        (void);

private:
  void      Setup       (void);
};  //BeckGyro
#endif  //BECKGYRO_H
