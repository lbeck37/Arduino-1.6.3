// BeckAlexaLib.cpp 2/17/19a
#include <BeckAlexaLib.h>
#include <BeckBiotaLib.h>
#include <BeckLogLib.h>
#include <BeckSwitchLib.h>
#include <BeckThermoLib.h>
#include <BeckE8266AccessPointLib.h>

int            wAlexaHandleCount     = 0;      //Incremented each time HandleAlexa() called
bool           bAlexaOn              = false;  //Only projects that use Alexa set this true.
fauxmoESP      Alexa;                          //Alexa emulation of Phillips Hue Bulb

void SetupAlexa(int wProjectType){
  String szLogString= "SetupAlexa(): Begin";
  LogToSerial(szLogString);
/*
  switch (wProjectType){
    case sFireplace:
    case sThermoDev:
      //Only these projects use Alexa
      bAlexaOn= true;
      break;
    default:
      bAlexaOn= false;
      break;
  } //switch
*/
  bAlexaOn= true;
  if(bAlexaOn){
    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    //Beck 12/3/18 I don't know why we do enable, disable, enable but it is necessary
    Alexa.enable(true);
    Alexa.enable(false);
    Alexa.enable(true);

    // You can use different ways to invoke Alexa to modify the devices state:
    // "Alexa, turn light one on" ("light one" is the name of the first device below)
    // "Alexa, turn on light one"
    // "Alexa, set light one to fifty" (50 means 50% of brightness)

    // Add virtual devices
    Alexa.addDevice(szAlexaName);

    //Define the callback
    Alexa.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value)
      {
      DoAlexaCommand(device_id, device_name, state, value);
      } );  //Alexa.onSetState
  } //if(bAlexaOn)
  else{
    szLogString = "SetupAlexa(): Alexa is not enabled for project ";
    LogToSerial(szLogString, wProjectType);
  } //if(bAlexaOn)else
  return;
} //SetupAlexa


void HandleAlexa(){
  wAlexaHandleCount++;  //Track how many times this is called before next handle system (10 sec)
  Alexa.handle();
  return;
} //HandleAlexa


void DoAlexaCommand(unsigned char ucDdeviceID, const char* szDeviceName, bool bState, unsigned char ucValue){
  char    szCharString[100];
  sprintf(szCharString, " DoAlexaCommand(): Device #%d (%s) bState: %s value: %d",
      ucDdeviceID, szDeviceName, (bState ? "ON " : "OFF"), ucValue);
  String szLogString= szCharString;
  LogToSerial(szLogString);
  SetAlexaSwitch(bState);
  fSetThermoSetpoint((int)ucValue);
  return;
} //DoAlexaCommand
//Last line.
