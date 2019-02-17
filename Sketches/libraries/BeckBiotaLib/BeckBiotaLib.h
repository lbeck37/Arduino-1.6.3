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

extern char           _acHostname   [50];
extern char           _acProjectType[50];
extern ProjectType    _eProjectType;

void  SetupSystem   (ProjectType eProjectType);

//Last line.
