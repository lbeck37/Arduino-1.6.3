// BeckBiotaLib.h 2/17/19a
#pragma once
#include <BeckBiotaLib.h>

enum ProjectType{
  eNoProject  = 0,
  eThermoDev,
  eFireplace,
  eGarage,
  eHeater,
  eFrontLights,
  eSlopeMeter,
  eLastProjectType
} ;

extern ProjectType _eProjectType;
//Last line.
