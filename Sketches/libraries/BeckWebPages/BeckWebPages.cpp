//BeckAngularTestPages.cpp, 5/5/19c
#include <BeckWebPages.h>
#include "BeckWebPagesHTML.h"
#include <BeckMiniLib.h>
#include <BeckWebServer.h>
#include <ArduinoJson.h>

double _dLastDegF      = 70.37;
double _dSetpointF     = 71.00;
double _dMaxHeatRangeF = 00.10;
double _dThermoOffDegF = _dSetpointF + _dMaxHeatRangeF;

// Enough space for:
// + 1 object with 6 members
const int wJsonCapacity = JSON_OBJECT_SIZE(6);

//For GET
StaticJsonDocument<wJsonCapacity>     oGetJsonDoc;
StaticJsonDocument<wJsonCapacity>     oPostJsonDoc;
char      szJsonText[128];


void ChangeDataValues() {
  Serial << LOG0 << "ChangeDataValues(): Begin" << endl;
  static const double dChange= 0.01;

  _dLastDegF       += dChange;
  _dSetpointF      += dChange;
  _dMaxHeatRangeF  += (dChange / 10.0);
  _dThermoOffDegF   = _dSetpointF + _dMaxHeatRangeF;
  return;
}

void HandleThermoDataGet() {
  ChangeDataValues();
  //Add args to Json Doc
  oGetJsonDoc["dLastDegF"]        = _dLastDegF;
  oGetJsonDoc["dSetpointF"]       = _dSetpointF;
  oGetJsonDoc["dMaxHeatRangeF"]   = _dMaxHeatRangeF;

  Serial << LOG0 << "HandleThermoDataGet(): Call serializeJson()" << endl;
  serializeJson(oGetJsonDoc, szJsonText);
  Serial << LOG0 << "HandleThermoDataGet(): Sending " << szJsonText << endl;

  oWebServer.send(200, "text/plain", szJsonText);
  return;
} //HandleThermoDataGet


void HandleThermoDataPost() {
  const String szPlain= oWebServer.arg("plain");
  Serial << LOG0 << "HandleThermoDataPost(): Received " << szPlain << endl;
  DeserializationError oDeserialError= deserializeJson(oPostJsonDoc, szPlain);
  if(oDeserialError != DeserializationError::Ok){
    Serial << LOG0 << "HandleThermoDataPost(): ERROR: oDeserialError is NOT DeserializationError::Ok " << endl;
  }
  else{
    oWebServer.send(200, "text/plain", "200: Good POST");
  }

  _dLastDegF      = oPostJsonDoc["dLastDegF"];
  _dSetpointF     = oPostJsonDoc["dSetpointF"];
  _dMaxHeatRangeF = oPostJsonDoc["dMaxHeatRangeF"];

  _dThermoOffDegF= _dSetpointF + _dMaxHeatRangeF;
  //Serial << LOG0 << "HandleThermoDataPost(): _dLastDegF, _dSetpointF, _dMaxHeatRangeF, _dThermoOffDegF: " << endl;
  //Serial << LOG0 << "HandleThermoDataPost(): " << _dLastDegF << ", " << _dSetpointF << ", " << _dMaxHeatRangeF << ", " << _dThermoOffDegF << endl;
  return;
} //HandleThermoDataPost


void SetupTermostatWebPage(){
  Serial << LOG0 << "SetupTermostatWebPage(): Set up handlers" << endl;
  oWebServer.on("/Thermostat", HTTP_GET, [](){
    Serial << LOG0 << "SetupTermostatWebPage(): Got a GET on /Thermostat, sending acThermostatTestPagesHTML[]" << endl;
    oWebServer.sendHeader("Connection", "close");
    //oWebServer.send(200, "text/html", acThermostatTestPagesHTML);
    oWebServer.send(200, "text/html", acThermoWebPageHTML);
    return;
  });

  oWebServer.on("/ThermoGet", HTTP_GET, [](){
    Serial << LOG0 << "SetupTermostatWebPage(): Got a GET on /ThermoGet" << endl;
    HandleThermoDataGet();
  });

  oWebServer.on("/ThermoPost", HTTP_POST, [](){
    Serial << LOG0 << "SetupTermostatWebPage(): Got a POST on /ThermoPost" << endl;
    HandleThermoDataPost();
    return;
  });

  oWebServer.on("/LastDegF", HTTP_GET, [](){
    Serial << LOG0 << "SetupTermostatWebPage(): Got a GET on /LastDegF" << endl;
    //oWebServer.send(200, "text/plain", szLastDegF().c_str());
    //oWebServer.send(200, "text/plain", szDummyDegF().c_str());
    //oWebServer.send(200, "text/plain", fDummyDegF());
    return;
  });

  return;
} //SetupTermostatWebPage
//Last line.
