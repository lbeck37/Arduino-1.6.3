// BeckBiotaLib.h 2/17/19b
#pragma once
//#include <BeckBiotaLib.h>

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
extern char          _acRouterName[];
extern char          _acRouterPW[];
extern char          _acAccessPointSSID[];
extern char          _acAccessPointPW[];
extern bool          _bSystemOk;

extern bool           _bSystemOk;

bool  SetupSystem   (ProjectType eProjectType);
//Last line.
