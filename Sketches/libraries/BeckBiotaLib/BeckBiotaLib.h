// BeckBiotaLib.h 3/4/19a
#pragma once

enum ProjectType{
  eNoProject  = 0,
  eThermoDev,
  eFireplace,
  eGarage,
  eHeater,
  eFrontLights,
  eIMU,
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
