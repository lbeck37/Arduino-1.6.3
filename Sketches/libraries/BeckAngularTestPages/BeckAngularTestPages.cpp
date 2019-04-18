//BeckAngularTestPages.cpp, 4/17/19a
#include <BeckAngularTestPages.h>
#include "BeckAngularTestPagesHTML.h"
#include <BeckMiniLib.h>
#include <BeckWebServer.h>

float _fLastDegF      = 70.37;
float _fSetpointF     = 71.00;
float _fThermoOffDegF = 71.10;

String CallBackFunc(const String& var){
  if(var == "TEMPERATURE"){
    return "99.99";
  }
  else if(var == "SET_POINT"){
    return "99.99";
  }
  else if(var == "THERMO_OFF"){
    return "99.99";
  }
  return String();
} //CallBackFunc


String szLastDegF() {
  return(String(_fLastDegF, 2));
} //szLastDegF


String szSetPointDegF() {
  return(String(_fSetpointF, 2));
} //szSetPointDegF


String szTermoOffDegF() {
  return(String(_fThermoOffDegF, 2));
} //szTermoOffDegF


void SetupAngularTestPages(){
  Serial << LOG0 << "SetupAngularTestPages(): Begin" << endl;

  Serial << LOG0 << "StartAsyncWebServer(): Set up handlers" << endl;
  oWebServer.on("/", HTTP_GET, [](){
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/html", acAngularTestPagesHTML);
  });

  oWebServer.on("/LastDegF", HTTP_GET, [](){
    oWebServer.send(200, "text/plain", szLastDegF().c_str());
  });

  oWebServer.on("/SetPointDegF", HTTP_GET, [](){
    oWebServer.send(200, "text/plain", szSetPointDegF().c_str());
  });

  oWebServer.on("/TermoOffDegF", HTTP_GET, [](){
    oWebServer.send(200, "text/plain", szTermoOffDegF().c_str());
  });

  return;
} //SetupAngularTestPages
//Last line.
