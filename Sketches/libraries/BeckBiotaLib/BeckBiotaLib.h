// BeckBiotaLib.h 3/5/19a
#pragma once
//#include <BeckMPU9150Lib.h>

enum ProjectType{
  eNoProject  = 0,
  eThermoDev,
  eFireplace,
  eGarage,
  eHeater,
  eFrontLights,
  ePitchMeter,
  eLastProjectType
} ;

extern char           _acHostname   [50];
extern char           _acProjectType[50];
extern char          _acRouterName[];
extern char          _acRouterPW[];
extern char          _acAccessPointSSID[];
extern char          _acAccessPointPW[];
extern bool          _bSystemOk;
extern ProjectType   _eProjectType;

bool  SetupSystem   (ProjectType eProjectType);
//Last line.
