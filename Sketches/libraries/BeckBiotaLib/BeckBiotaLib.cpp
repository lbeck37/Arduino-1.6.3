// BeckBiotaLib.cpp 4/11/19a
#include <BeckBiotaLib.h>

ProjectType   _eProjectType;
char          _acHostname       [50];
char          _acProjectType    [50];
char          _acRouterName     [50];
char          _acRouterPW       [50];
char          _acAccessPointSSID[50];
char          _acAccessPointPW  [50];
bool          _bSystemOk      = true;


bool SetupSystem(ProjectType eProjectType){
  Serial << LOG0 << "SetupSystem(): Begin" << endl;
  bool  bOk= true;
  _eProjectType= eProjectType;
  switch (_eProjectType){
    case eThermoDev:
      strcpy(_acHostname        , "BeckThermoDev");
      strcpy(_acProjectType     , "THERMO_DEV");
      strcpy(_acAlexaName       , "Larry's Device");
      strcpy(_acRouterName      , "Aspot24");
      strcpy(_acRouterPW        , "Qazqaz11");
      strcpy(_acAccessPointSSID , "BiotaSpot");
      strcpy(_acAccessPointPW   , "Qazqaz11");
      _fSetpointF       =  75.0;
      _fMinSetpoint     =  20.0;
      _fMaxSetpoint     =  99.0;
      _fMaxHeatRangeF   = 0.10;
      _fThermoOffDegF   = _fSetpointF + _fMaxHeatRangeF;
      break;
    case eFireplace:
      strcpy(_acHostname        , "BeckFireplace");
      strcpy(_acProjectType     , "FIREPLACE");
      strcpy(_acAlexaName       , "Fireplace");
      strcpy(_acRouterName      , "Aspot24");
      strcpy(_acRouterPW        , "Qazqaz11");
      strcpy(_acAccessPointSSID , "FireplaceSpot");
      strcpy(_acAccessPointPW   , "Qazqaz11");
      _fSetpointF       = 71.0;
      _fMinSetpoint     = 65.0;
      _fMaxSetpoint     = 75.0;
      _fMaxHeatRangeF   = 0.10;
      _fThermoOffDegF   = _fSetpointF + _fMaxHeatRangeF;
      break;
    case eHeater:
      strcpy(_acHostname        , "BeckHeater");
      strcpy(_acProjectType     , "HEATER");
      strcpy(_acAlexaName       , "Candy's Heater");
      strcpy(_acRouterName      , "Aspot24");
      strcpy(_acRouterPW        , "Qazqaz11");
      strcpy(_acAccessPointSSID , "HeaterSpot");
      strcpy(_acAccessPointPW   , "Qazqaz11");
      _fSetpointF       = 75.0;
      _fMinSetpoint     = 55.0;
      _fMaxSetpoint     = 80.0;
      _fMaxHeatRangeF   = 0.10;
      _fThermoOffDegF   = _fSetpointF + _fMaxHeatRangeF;
      break;
    case eGarage:
      strcpy(_acHostname        , "BeckGarage");
      strcpy(_acProjectType     , "GARAGE");
      strcpy(_acAlexaName       , "Garage");
      strcpy(_acRouterName      , "Aspot24");
      strcpy(_acRouterPW        , "Qazqaz11");
      strcpy(_acAccessPointSSID , "GarageSpot");
      strcpy(_acAccessPointPW   , "Qazqaz11");
      _fSetpointF       = 35.0;
      _fMinSetpoint     = 33.0;
      _fMaxSetpoint     = 50.0;
      _fMaxHeatRangeF   = 1.00;
      _fThermoOffDegF   = _fSetpointF + _fMaxHeatRangeF;
      break;
    case ePitchMeter:
      strcpy(_acHostname        , "BeckPitch");
      strcpy(_acProjectType     , "PITCH");
      strcpy(_acAlexaName       , "Pitch Meter");
      //strcpy(_acRouterName      , "Aspot24");
      strcpy(_acRouterName      , "Dspot");
      strcpy(_acRouterPW        , "Qazqaz11");
      strcpy(_acAccessPointSSID , "PitchSpot");
      strcpy(_acAccessPointPW   , "Qazqaz11");
      break;
    case eFrontLights:
      strcpy(_acHostname        , "BeckFrontLights");
      strcpy(_acProjectType     , "FRONT_LIGHTS");
      strcpy(_acAlexaName       , "Front Lights");
      strcpy(_acRouterName      , "Aspot24");
      strcpy(_acRouterPW        , "Qazqaz11");
      strcpy(_acAccessPointSSID , "FrontLightsSpot");
      strcpy(_acAccessPointPW   , "Qazqaz11");
      break;
    case eNoProject:
    default:
      Serial << LOG0 << "SetupSystem(): Bad switch, _eProjectType= " << _eProjectType << endl;
      bOk= false;
      break;
  } //switch
  Serial << LOG0 << "SetupSystem(): Project Type set to: " << _acProjectType << endl;
  return bOk;
} //SetupSystem


void SwitchProjectType(ProjectType eProjectType){
  Serial << LOG0 << "SwitchProjectType(): Switch _eProjectType to: " << eProjectType << endl;
  _eProjectType= eProjectType;
  return;
} //SwitchProjectType
//Last line.
