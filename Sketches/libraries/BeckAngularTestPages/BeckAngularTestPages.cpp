//BeckAngularTestPages.cpp, 5/5/19a
#include <BeckAngularTestPages.h>
#include "BeckAngularTestPagesHTML.h"
#include <BeckMiniLib.h>
#include <BeckWebServer.h>
#include <ArduinoJson.h>

double _dLastDegF      = 70.37;
double _dSetpointF     = 71.00;
double _dMaxHeatRangeF = 00.10;
double _dThermoOffDegF = _dSetpointF + _dMaxHeatRangeF;

//Function protos
void HandleTestRoot   ();
void HandleLoginRoot  ();
void HandleTestFunc   ();
void HandleLoginFunc  ();
//void HandleNotFound   ();

// Enough space for:
// + 1 object with 3 members
const int wJsonCapacity = JSON_OBJECT_SIZE(3);

//For GET
StaticJsonDocument<wJsonCapacity>     oGetJsonDoc;
StaticJsonDocument<wJsonCapacity>     oPostJsonDoc;
char      szJsonText[128];
/*
//For POST
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
StaticJsonBuffer<200> oStaticJsonBuffer;
JsonObject& pJsonObject = oStaticJsonBuffer.parseObject(server.arg("plain"));
*/


void SetupAngularTestPages(){
  Serial << LOG0 << "SetupAngularTestPages(): Begin" << endl;

  Serial << LOG0 << "SetupAngularTestPages(): Set up handlers" << endl;
  oWebServer.on("/ajs3", HTTP_GET, [](){
    Serial << LOG0 << "SetupAngularTestPages(): Got a GET on /ajs" << endl;
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/html", acAngularTestPagesHTML);
  });
  return;
} //SetupAngularTestPages


void SetupFormTestPages(void){
  oWebServer.on("/"            , HTTP_GET,   HandleTestRoot);
  oWebServer.on("/login"       , HTTP_GET,   HandleLoginRoot);
  oWebServer.on("/handlelogin" , HTTP_POST,  HandleLoginFunc);
  oWebServer.on("/handletest"  , HTTP_POST,  HandleTestFunc);

  //oWebServer.onNotFound(HandleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  oWebServer.begin();                            // Actually start the server
  Serial << endl << LOG0 << "setup(): HTTP Web Server started" << endl;
  return;
} //SetupFormTestPages


void HandleTestRoot() {
  oWebServer.send(200, "text/html", "<form action=\"/handletest\" method=\"POST\"><input type=\"text\" name=\"word1\" placeholder=\"Word #1\"></br><input type=\"text\" name=\"word2\" placeholder=\"Word #2\"></br><input type=\"submit\" value=\"Next\"></form><p>Enter words for test</p>");
  return;
} //HandleTestRoot


void HandleLoginRoot() {                          // When URI / is requested, send a web page with a button to toggle the LED
  //oWebServer.send(200, "text/html", "<form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'dude' and '111' ...</p>");
  oWebServer.send(200, "text/html", "<form action=\"/handlelogin\" method=\"POST\"><input type=\"text\" name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'dude' and '111' ...</p>");
  return;
} //HandleLoginRoot


void HandleTestFunc() {
  Serial << endl << LOG0 << "HandleTestFunc(): Received word1= " << oWebServer.arg("word1") <<
      ", password= " << oWebServer.arg("word2") << endl;

  oWebServer.send(200, "text/html", "<h1>Thanks for entering <i>" + oWebServer.arg("word1") + "</i> <i>" + oWebServer.arg("word2") + "</i>, You Rock!</h1><p>Please proceed to the next fork in the road and take it. <i>YB</i></p>");
  return;
} //HandleTestFunc


void HandleLoginFunc() {                         // If a POST request is made to URI /login
  if( ! oWebServer.hasArg("username") || ! oWebServer.hasArg("password")
      || oWebServer.arg("username") == NULL || oWebServer.arg("password") == NULL) { // If the POST request doesn't have username and password data
    oWebServer.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }

  if(oWebServer.arg("username") == "dude" && oWebServer.arg("password") == "111") { // If both the username and the password are correct
    oWebServer.send(200, "text/html", "<h1>Welcome, " + oWebServer.arg("username") + "!</h1><p>Login successful</p>");
  } else {                                                                              // Username and password don't match
    oWebServer.send(401, "text/plain", "401: Unauthorized");
  }
  return;
} //HandleLoginFunc

/* Actual original HTML text
<form action="/login" method="POST">
    <input type="text" name="username" placeholder="Username"></br>
    <input type="password" name="password" placeholder="Password"></br>
    <input type="submit" value="Login">
</form>
<p>
    Try 'dude' and '111' ...
</p>
*/

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
} //CallBackFunc GetThermoData


String szLastDegF() {
  return(String(_dLastDegF, 2));
} //szLastDegF


double dChange= 0.01;
void HandleThermoDataGet() {
  Serial << LOG0 << "HandleThermoDataGet(): Begin" << endl;

  _dLastDegF       += dChange;
  _dSetpointF      += dChange;
  _dMaxHeatRangeF  += dChange / 10;
  _dThermoOffDegF   = _dSetpointF + _dMaxHeatRangeF;

  //Add args to Json Doc
  oGetJsonDoc["dDegF"]          = _dLastDegF;
  oGetJsonDoc["dSetpoint"]      = _dSetpointF;
  oGetJsonDoc["dMaxHeatRange"]  = _dMaxHeatRangeF;
  //dChange += 0.01;

  serializeJson(oGetJsonDoc, szJsonText);
  oWebServer.send(200, "text/plain", szJsonText);
  return;
} //HandleThermoDataGet


bool bSaveThermoPostArgs() {
  bool  bReturn= true;
  String szPlain= oWebServer.arg("plain");
  Serial << LOG0 << "bSaveThermoPostArgs(): oWebServer.arg(" << "'plain'" << ") is " << szPlain << endl;

  //double dDegF= oPostJsonDoc["dDegF"];
  String szDegF= oPostJsonDoc["dDegF"];
  Serial << LOG0 << "bSaveThermoPostArgs(): Received szDegF= " << szDegF << endl;

  double dSetpoint= oPostJsonDoc["dSetpoint"];
  Serial << LOG0 << "bSaveThermoPostArgs(): Received dSetpoint= " << dSetpoint << endl;

  double dMaxHeatRange= oPostJsonDoc["dMaxHeatRange"];
  Serial << LOG0 << "bSaveThermoPostArgs(): Received dMaxHeatRange= " << dMaxHeatRange << endl;

  //_dLastDegF      = oPostJsonDoc["dDegF"];
  //_dSetpointF     = oPostJsonDoc["dSetpoint"];
  //_dMaxHeatRangeF = oPostJsonDoc["dMaxHeatRange"];
  _dLastDegF      = 37.0;
  _dSetpointF     = dSetpoint;
  _dMaxHeatRangeF = dMaxHeatRange;

  //Don't use posted _dThermoOffDegF value
  _dThermoOffDegF   = _dSetpointF + _dMaxHeatRangeF;
  return bReturn;
} //bSaveThermoPostArgs


void HandleThermoDataPost() {
  Serial << LOG0 << "HandleThermoDataPost(): Begin" << endl;
  bSaveThermoPostArgs();
  oWebServer.send(200, "text/plain", "200: 10-4 Good buddy");
  return;
} //HandleThermoDataPost


String szDummyDegF() {
  static float    fCurrent= 0.37;

  float fReturn= fCurrent;
  fCurrent += 1.00;
  return(String(fReturn, 2));
} //szDummyDegF


float fDummyDegF() {
  static float    fCurrent= 0.37;

  float fReturn= fCurrent;
  fCurrent += 1.00;
  return(fReturn);
} //fDummyDegF


String szSetPointDegF() {
  return(String(_dSetpointF, 2));
} //szSetPointDegF


String szTermoOffDegF() {
  return(String(_dThermoOffDegF, 2));
} //szTermoOffDegF


void SetupTermostatTestPages(){
  Serial << LOG0 << "SetupTermoTestPages(): Begin" << endl;

  Serial << LOG0 << "SetupTermoTestPages(): Set up handlers" << endl;
  oWebServer.on("/Thermostat", HTTP_GET, [](){
    oWebServer.sendHeader("Connection", "close");
    oWebServer.send(200, "text/html", acThermostatTestPagesHTML);
    Serial << endl << LOG0 << "SetupTermostatTestPages(): Servicing HTTP_GET at /Thermostat" << endl;
  });

  oWebServer.on("/ThermoGet", HTTP_GET, [](){
    Serial << LOG0 << "SetupTermoTestPages(): Got a GET on /ThermoGet" << endl;
    HandleThermoDataGet();
  });

  oWebServer.on("/ThermoPost", HTTP_POST, [](){
    Serial << LOG0 << "SetupTermoTestPages(): Got a POST on /ThermoPost" << endl;
    HandleThermoDataPost();
  });

  oWebServer.on("/LastDegF", HTTP_GET, [](){
    Serial << LOG0 << "SetupTermoTestPages(): Got a GET on /LastDegF" << endl;
    //oWebServer.send(200, "text/plain", szLastDegF().c_str());
    oWebServer.send(200, "text/plain", szDummyDegF().c_str());
    //oWebServer.send(200, "text/plain", fDummyDegF());
  });

  oWebServer.on("/SetPointDegF", HTTP_GET, [](){
    oWebServer.send(200, "text/plain", szSetPointDegF().c_str());
  });

  oWebServer.on("/TermoOffDegF", HTTP_GET, [](){
    oWebServer.send(200, "text/plain", szTermoOffDegF().c_str());
  });

  return;
} //SetupTermoTestPages
//Last line.
