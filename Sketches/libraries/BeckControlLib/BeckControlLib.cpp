//BeckControlLib.cpp

#include <BeckLib.h>
#include <BeckControlLib.h>

#include <OneWire.h>

#ifndef ESP32
  #include <Adafruit_ADS1015.h>
#endif

#define ONEWIRE_PIN       12

const int    sSwitchOpen           = 0;
const int    sSwitchClosed         = 1;
const int    sOff                  = 0;
const int    sOn                   = 1;
const int    sNotInit              = -3737;

const int    sNumSwitches          = 4;
const int    sFurnaceSwitchNum     = 2;      //Was 1, switch number that turns furnace on and off.
const long   sThermoTimesInRow     = 3;      //Max times temp is outside range before switch
const float  fMaxHeatRangeF        = 2.00;   //Temp above setpoint before heat is turned off

int          asSwitchState[]       = {0, 0, 0, 0, 0};
int          asSwitchLastState[]   = {sNotInit, sNotInit, sNotInit, sNotInit, sNotInit};
float        fLastDegF             = 37.37;  //Last temperature reading.
int          sSetpointF            = 37;
int          sThermoTimesCount     = 0;      //Number of times temperature out of range
bool         bThermoOn             = true;   //Whether thermostat is running.
bool         bFurnaceOn            = false;  //If switch is on to turn on furnace.
float        fThermoOffDegF        = sSetpointF + fMaxHeatRangeF;

const int    asSwitchPin[]         = {-1, 4, 5, 15, 16};    //0 is not a switch, switches are at 1,2,3,4
const bool   abSwitchInverted[]    = {0, true, true, true, true};  //Opto-isolated relays close when pulled low.
const int    sThermoDummySwitch    = 0;  //Thermostat Blynk LED lives at unused switch #0.
const int    sOneWirePin           = ONEWIRE_PIN;  //Dallas DS18B20 Temperature Sensor

//Maxim/Dallas OneWire sensors
/*Set up a oneWire instance to communicate with any OneWire device*/
OneWire         oOneWire(sOneWirePin);

/*Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature   oSensors(&oOneWire);

#ifdef ESP8266
  Adafruit_ADS1115  AtoD(0x48);
#endif

//ESP32 AtoD Input Pins
const int	sNumTanks		= 3;
const int	sPinsPerTank		= 2;

const uint8_t ucGrey1PowerPin		= 34;
const uint8_t ucGrey1LevelPin		= 35;
const uint8_t ucBlackPowerPin		= 25;
const uint8_t ucBlackLevelPin		= 26;
const uint8_t ucGrey2PowerPin		= 27;
const uint8_t ucGrey2LevelPin		= 14;

const uint8_t ucTankPin[sNumTanks][sPinsPerTank]=
	{
		{ucGrey1PowerPin, ucGrey1LevelPin},
		{ucBlackPowerPin, ucBlackLevelPin},
		{ucGrey2PowerPin, ucGrey2LevelPin}
	};

/****************************************************************/
void SetupAtoD(){
#if 1 //ESP32
	for (int sTank= 0; sTank < sNumTanks; sTank++) {
		for (int sPin= 0; sPin < sPinsPerTank; sPin++) {
			  pinMode(ucTankPin[sTank][sPinsPerTank], INPUT);
		}	//for (int sPin= 0;...
	}	//for (int sTank= 0;...
#endif	//ESP32

#if ESP8266
  String szLogString="SetupAtoD(): Call AtoD.begin()";
  LogToBoth(szLogString);
  AtoD.begin();
  szLogString="SetupAtoD(): Call AtoD.begin()";
  LogToBoth(szLogString);
#endif	//ESP8266
  return;
} //SetupAtoD


float fReadAtoD(int sInputPin){
  float fVoltage= 0.370;
#if 1 //ESP32
  uint8_t ucPin= (uint8_t)sInputPin;
  String szLogString = "setup(): analogRead(ucPin)";
  LogToBoth(szLogString);

  int sValue= analogRead(ucPin);
  szLogString = "setup(): sValue= ";
  LogToBoth(szLogString, sValue);
  fVoltage = (sValue * 1.61)/1000;	//12-bit AtoD +/-2048, assume 3.3V max
#endif

#ifdef ESP8266
  String szLogString="fReadAtoD(): Ch=";
  LogToBoth(szLogString, sInputPin);
	int sAtoDReading = AtoD.readADC_SingleEnded(sInputPin);
  szLogString="fReadAtoD():";
  LogToBoth(szLogString, sAtoDReading);
	//Convert 16bit value from the AtoD into volts
	fVoltage = (sAtoDReading * 0.1875)/1000;
#endif
  return  fVoltage;
} //fReadAtoD


void HandleFurnaceSwitch(){
  String szLogString = "HandleFurnaceSwitch(): bFurnaceOn";
  LogToBoth(szLogString, bFurnaceOn);
  //Serial << LOG0 << "HandleFurnaceSwitch(): bThermoOn, bFurnaceOn " << bThermoOn << ", " << bFurnaceOn << endl;
  //Make sure  switch state represents bHeatOn correctly.
  if (bFurnaceOn){
    //Serial << LOG0 << "HandleFurnaceSwitch(): Set asSwitchState[sFurnaceSwitchNum] to sOn" << endl;
    asSwitchState[sFurnaceSwitchNum]= sOn;
  } //if(bFurnaceOn)
  else{
    //Serial << LOG0 << "HandleFurnaceSwitch(): Set asSwitchState[sFurnaceSwitchNum] to sOff" << endl;
    asSwitchState[sFurnaceSwitchNum]= sOff;
  } //if(bFurnaceOn)else
  SetSwitch(sFurnaceSwitchNum, asSwitchState[sFurnaceSwitchNum]);
  return;
} //HandleFurnaceSwitch


void TurnFurnaceOn(bool bTurnOn){
  if (bTurnOn){
    String szLogString= "TurnFurnaceOn(): Furnace turned ON";
    LogToBoth(szLogString);
    bFurnaceOn= true;
    SetFurnaceSwitch(sSwitchClosed);
    sThermoTimesCount= 0;
  } //if(bTurnOn)
  else{
    String szLogString= "TurnFurnaceOn(): Furnace turned OFF";
    LogToBoth(szLogString);
    bFurnaceOn= false;
    SetFurnaceSwitch(sSwitchOpen);
    sThermoTimesCount= 0;
  } //if(bTurnOn)else
  return;
} //TurnFurnaceOn


void SetThermoState(int sSwitchState){
  asSwitchState[sThermoDummySwitch]= sSwitchState;
  if (sSwitchState == sOn){
    bThermoOn= true;
  } //if(sState==sOn)
  else{
    bThermoOn= false;
    bFurnaceOn= false;
    sThermoTimesCount= 0;
    SetFurnaceSwitch(sSwitchOpen);
  } //if(sState==sOn)else
  return;
} //SetThermoState


void SetFurnaceSwitch(int sSwitchState){
  SetSwitch(sFurnaceSwitchNum, sSwitchState);
  return;
} //SetFurnaceSwitch


void SetSwitch(int sSwitch, int sSwitchState){
  int sSwitchPin= asSwitchPin[sSwitch];
  bool bPinSetting;
  asSwitchState[sSwitch]= sSwitchState;
  //bDebugLog= bDebug;
  if (abSwitchInverted[sSwitch]){
    bPinSetting= !sSwitchState;
  } //if(abSwitchInverted[sSwitch])
  else{
    bPinSetting= sSwitchState;
  } //if(abSwitchInverted[sSwitch])else
  //Serial << LOG0 << "SetSwitch(): sSwitch, sSwitchState, sSwitchPin, bPinSetting " << sSwitch << ", " << sSwitchState << ", " << sSwitchPin << ", " << bPinSetting <<  endl;
  //Serial << LOG0 << "SetSwitch(): sSwitch, sSwitchState, sSwitchPin, bPinSetting" <<  endl;
  String szLogString= "SetSwitch:  ";
  szLogString += sSwitch;
  szLogString += ",";
  szLogString += sSwitchState;
  szLogString += ",";
  szLogString += sSwitchPin;
  szLogString += ",";
  szLogString += bPinSetting;
  //LogToBoth(szLogString);

  //Some switch positions don't have pins, just Blynk LEDs.
  if (sSwitchPin >= 0){
    digitalWrite(sSwitchPin, bPinSetting);
    asSwitchState[sSwitch]= sSwitchState;
  } //if(sSwitchPin>=0)
  //bDebugLog= true;
  //HandleBlynkLEDs();
  return;
} //SetSwitch


float fGetDegF(bool bTakeReading){
  float fDegFReturn= 37.37;   //Value used for default in testing w/o reading sensor. fLastDegF
  if (bTakeReading){
    oSensors.requestTemperatures(); // Send the command to get temperatures
    fDegFReturn= oSensors.getTempFByIndex(0);
    fLastDegF= fDegFReturn;
  } //if(bTakeReading)
  else{
    fDegFReturn= fLastDegF;
  } //if(bTakeReading)else
  return fDegFReturn;
}  //fGetDegF
