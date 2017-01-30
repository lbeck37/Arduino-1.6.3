//BeckControlLib.cpp

#include <BeckLib.h>
#include <BeckControlLib.h>

#include <OneWire.h>

#ifdef ESP8266
  #include <Adafruit_ADS1015.h>
  Adafruit_ADS1115  AtoD(0x48);
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

#ifdef ESP8266
  const int    sOneWirePin           = 12;  //Dallas DS18B20 Temperature Sensor
  //Maxim/Dallas OneWire sensors
  /*Set up a oneWire instance to communicate with any OneWire device*/
  OneWire         oOneWire(sOneWirePin);

  /*Tell Dallas Temperature Library to use oneWire Library */
  DallasTemperature   oSensors(&oOneWire);
#endif	//ESP8266

//ESP32 AtoD Input Pins for tank monitoring
const int	sNumTanks		= 3;
const int	sPinsPerTank		= 2;

const uint8_t ucGrey1PowerPin		= 34;
const uint8_t ucGrey1LevelPin		= 35;	//Blynk V6
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

//List of all possible ADCs for testing
#define TEST_ADCs
const uint8_t ucADC_0			= 99;	//36; Preamp pins, skip testing
const uint8_t ucADC_1			= 99;	//37;
const uint8_t ucADC_2			= 99;	//38;
const uint8_t ucADC_3			= 99;	//39;
const uint8_t ucADC_4			= 32;

const uint8_t ucADC_5			= 33;
const uint8_t ucADC_6			= 34;
const uint8_t ucADC_7			= 35;
const uint8_t ucADC_18			= 25;
const uint8_t ucADC_19			= 26;

const uint8_t ucADC_17			= 27;
const uint8_t ucADC_16			= 14;
const uint8_t ucADC_15			= 12;
const uint8_t ucADC_14			= 13;
const uint8_t ucADC_13			= 15;

const uint8_t ucADC_12			= 99;	//2;	//
const uint8_t ucADC_11			= 99;	//0;
const uint8_t ucADC_10			= 4;

const int sNumADCpins			= 18;

const uint8_t ucADC_Pins[sNumADCpins]= {
    ucADC_0, ucADC_1, ucADC_2, ucADC_3, ucADC_4,
    ucADC_5, ucADC_6, ucADC_7, ucADC_18, ucADC_19,
    ucADC_17, ucADC_16, ucADC_15, ucADC_14, ucADC_13,
    ucADC_12, ucADC_11, ucADC_10};

//Local function protos
void SetupAllADCs();
void TestAllADCs();

/****************************************************************/
void SetupAtoD(){
#if 1 //ESP32
#ifndef TEST_ADCs
  for (int sTank= 0; sTank < sNumTanks; sTank++) {
    for (int sPin= 0; sPin < sPinsPerTank; sPin++) {
      pinMode(ucTankPin[sTank][sPinsPerTank], INPUT);
    }	//for (int sPin= 0;...
  }	//for (int sTank= 0;...
#else
  SetupAllADCs();
#endif	//TEST_ADCs
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
  //Run though all ADC'sif we are reading Grey1 Level.
  if (sInputPin == ucGrey1LevelPin) {
      TestAllADCs();
  }	//if(sInputPin==ucGrey1LevelPin)
  String szLogString = "fReadAtoD(): analogRead Pin:";
  LogToBoth(szLogString, ucPin);

  int sValue= analogRead(ucPin);
  szLogString = "fReadAtoD(): sValue= ";
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


void SetupAllADCs() {
  for (int sADCPin= 0; sADCPin < sNumADCpins; sADCPin++) {
      uint8_t ucPin= ucADC_Pins[sADCPin];
      if (ucPin != 99) {
	String szLogString = "SetupAllADCs(): Pin set to input";
	LogToBoth(szLogString, ucPin);
	pinMode(ucPin, INPUT);
      }	//if(ucPin!=99)
    }	//for (int sADCPin= 0;...
  return;
}  //SetupAllADCs


void TestAllADCs() {
  String szLogString = "TestAllADCs(): Begin";
  LogToBoth(szLogString);
  for (int sIndex= 0; sIndex < sNumADCpins;sIndex++) {
      uint8_t ucPin= ucADC_Pins[sIndex];
      szLogString = "TestAllADCs(): analogRead Pin:";
      LogToBoth(szLogString, ucPin);
      //Some pins are not tested and their pin mumbers are set to 99
      if (ucPin != 99) {
	int sValue= analogRead(ucPin);
	szLogString = "TestAllADCs(): sValue= ";
	LogToBoth(szLogString, sValue);
	float fVoltage = (sValue * 0.1875)/1000;
	szLogString = "TestAllADCs(): fVoltage= ";
	LogToBoth(szLogString, fVoltage);
	delay(100);
      }	//if(ucPin!=99)
  }	//for
  return;
}	//TestAllADCs


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
#ifdef ESP8266
  if (bTakeReading){
    oSensors.requestTemperatures(); // Send the command to get temperatures
    fDegFReturn= oSensors.getTempFByIndex(0);
    fLastDegF= fDegFReturn;
  } //if(bTakeReading)
  else{
    fDegFReturn= fLastDegF;
  } //if(bTakeReading)else
#endif
  return fDegFReturn;
}  //fGetDegF
