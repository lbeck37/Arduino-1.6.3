//BeckE8266AlexaLib.cpp 1/13/19
#include <BeckE8266AlexaLib.h>
#include <Streaming.h>
#include <BeckMiniLib.h>
#include <fauxmoESP.h>        //Alexa include file

bool        _bAlexaSwitchOn;
fauxmoESP   _Alexa;    //Alexa emulation of Phillips Hue Bulb

void DoAlexaCommand(unsigned char ucDdeviceID, const char* szDeviceName, bool bState, unsigned char ucValue){
  Serial << LOG0;
  Serial.printf(" DoAlexaCommand(): Device #%d (%s) bState: %s value: %d",
                  ucDdeviceID, szDeviceName, (bState ? "ON " : "OFF"), ucValue);
  _bAlexaSwitchOn= bState;
  Serial << "DoAlexaComman(): Return" << endl;
  return;
} //DoAlexaCommand


void StartAlexa(const char szAlexaName[]){
  Serial << LOG0 << "StartAlexa(): Begin" << endl;
  _bAlexaSwitchOn= false;

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  //Beck 12/3/18 I don't know why we do enable, disable, enable but it is necessary
  _Alexa.enable(true);
  _Alexa.enable(false);
  _Alexa.enable(true);

  // You can use different ways to invoke Alexa to modify the devices state:
  // "Alexa, turn light one on" ("light one" is the name of the first device below)
  // "Alexa, turn on light one"
  // "Alexa, set light one to fifty" (50 means 50% of brightness)

  // Add virtual devices
  _Alexa.addDevice(szAlexaName);

  //Define the callback
  _Alexa.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value)
    {
    DoAlexaCommand(device_id, device_name, state, value);
    } );  //_Alexa.onSetState
  return;
} //StartAlexa


void HandleAlexaLib(){
  _Alexa.handle();
  return;
} //HandleAlexaLib
