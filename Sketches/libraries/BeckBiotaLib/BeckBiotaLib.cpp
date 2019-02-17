// BeckBiotaLib.cpp 2/17/19a
#include <BeckBiotaLib.h>
#include <BeckAlexaLib.h>
#include <BeckLogLib.h>

char          _acHostname   [50];
char          _acProjectType[50];
ProjectType   _eProjectType;

void SetupSystem(ProjectType eProjectType){
  Serial << LOG0 << "SetupSystem(): Begin" << endl;
  _eProjectType= eProjectType;
  switch (_eProjectType){
    case eThermoDev:
      strcpy(_acHostname   , "BeckThermoDev");
      strcpy(_acProjectType, "THERMO_DEV");
      strcpy(_acAlexaName  , "Larry's Device");
      break;
    case eFireplace:
      strcpy(_acHostname   , "BeckFireplace");
      strcpy(_acProjectType, "FIREPLACE");
      strcpy(_acAlexaName  , "Fireplace");
      break;
    case eHeater:
      strcpy(_acHostname   , "BeckHeater");
      strcpy(_acProjectType, "HEATER");
      strcpy(_acAlexaName  , "Candy's Heater");
      break;
    case eGarage:
      strcpy(_acHostname   , "BeckGarage");
      strcpy(_acProjectType, "GARAGE");
      strcpy(_acAlexaName  , "Garage");
      break;
    case eSlopeMeter:
      strcpy(_acHostname   , "BeckSlopeMeter");
      strcpy(_acProjectType, "SLOPE_METER");
      strcpy(_acAlexaName  , "Slope Meter");
      break;
    case eFrontLights:
      strcpy(_acHostname   , "BeckFrontLights");
      strcpy(_acProjectType, "FRONT_LIGHTS");
      strcpy(_acAlexaName  , "Front Lights");
      break;
    case eNoProject:
    default:
      Serial << LOG0 << "SetupSystem(): Bad switch, _eProjectType= " << _eProjectType << endl;
      break;
  } //switch
  Serial << LOG0 << "SetupSystem(): Project Type set to: " << _acProjectType << endl;
  return;
} //SetupSystem
//Last line.
