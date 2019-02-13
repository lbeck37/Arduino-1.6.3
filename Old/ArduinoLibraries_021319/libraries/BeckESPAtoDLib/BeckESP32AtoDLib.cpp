// BeckESP32AtoDLib.cpp
#include <BeckESP32AtoDLib.h>

//ESP32 AtoD Input Pins for tank monitoring
const int sNumTanks       = 3;
const int sPinsPerTank    = 2;

const uint8_t ucGrey1PowerPin   = 34;
const uint8_t ucGrey1LevelPin   = 35; //Blynk V6
const uint8_t ucBlackPowerPin   = 25;
const uint8_t ucBlackLevelPin   = 26;
const uint8_t ucGrey2PowerPin   = 27;
const uint8_t ucGrey2LevelPin   = 14;

const uint8_t ucTankPin[sNumTanks][sPinsPerTank]=
  {
    {ucGrey1PowerPin, ucGrey1LevelPin},
    {ucBlackPowerPin, ucBlackLevelPin},
    {ucGrey2PowerPin, ucGrey2LevelPin}
  };

//List of all possible ADCs for testing
//#define TEST_ADCs
const uint8_t ucADC_0     = 99; //36; Preamp pins, skip testing
const uint8_t ucADC_1     = 99; //37;
const uint8_t ucADC_2     = 99; //38;
const uint8_t ucADC_3     = 99; //39;
const uint8_t ucADC_4     = 32;

const uint8_t ucADC_5     = 33;
const uint8_t ucADC_6     = 34;
const uint8_t ucADC_7     = 35;
const uint8_t ucADC_18      = 25;
const uint8_t ucADC_19      = 26;

const uint8_t ucADC_17      = 27;
const uint8_t ucADC_16      = 14;
const uint8_t ucADC_15      = 12;
const uint8_t ucADC_14      = 13;
const uint8_t ucADC_13      = 15;

const uint8_t ucADC_12      = 99; //2;  //
const uint8_t ucADC_11      = 99; //0;
const uint8_t ucADC_10      = 4;

const int sNumADCpins     = 18;

const uint8_t ucADC_Pins[sNumADCpins]= {
    ucADC_0, ucADC_1, ucADC_2, ucADC_3, ucADC_4,
    ucADC_5, ucADC_6, ucADC_7, ucADC_18, ucADC_19,
    ucADC_17, ucADC_16, ucADC_15, ucADC_14, ucADC_13,
    ucADC_12, ucADC_11, ucADC_10};

//Local function protos
void SetupAllADCs();
void TestAllADCs();

//Functions
void SetupESPAtoD(){
  #ifndef TEST_ADCs
    for (int sTank= 0; sTank < sNumTanks; sTank++) {
      for (int sPin= 0; sPin < sPinsPerTank; sPin++) {
  pinMode(ucTankPin[sTank][sPinsPerTank], INPUT);
      } //for (int sPin= 0;...
    } //for (int sTank= 0;...
  #else
    SetupAllADCs();
  #endif  //TEST_ADCs
  return;
} //SetupESPAtoD


float fReadESPAtoD(int sInputPin){
  float fVoltage= 0.370;
  uint8_t ucPin= (uint8_t)sInputPin;
  //Run though all ADC'sif we are reading Grey1 Level.
  if (sInputPin == ucGrey1LevelPin) {
      TestAllADCs();
  } //if(sInputPin==ucGrey1LevelPin)
  String szLogString = "fReadESPAtoD(): analogRead Pin:";
  LogToBoth(szLogString, ucPin);

  int sValue= analogRead(ucPin);
  szLogString = "fReadESPAtoD(): sValue= ";
  LogToBoth(szLogString, sValue);
  fVoltage = (sValue * 1.61)/1000;  //12-bit AtoD +/-2048, assume 3.3V max
  return  fVoltage;
} //fReadESPAtoD


void SetupAllADCs() {
  for (int sADCPin= 0; sADCPin < sNumADCpins; sADCPin++) {
      uint8_t ucPin= ucADC_Pins[sADCPin];
      if (ucPin != 99) {
  String szLogString = "SetupAllADCs(): Pin set to input";
  LogToBoth(szLogString, ucPin);
  pinMode(ucPin, INPUT);
      } //if(ucPin!=99)
    } //for (int sADCPin= 0;...
  return;
}  //SetupAllADCs


void TestAllADCs() {
  String szLogString = "TestAllADCs(): Begin";
  LogToBoth(szLogString);
  for (int sIndex= 0; sIndex < sNumADCpins;sIndex++) {
      uint8_t ucPin= ucADC_Pins[sIndex];
      szLogString = "TestAllADCs():Pin";
      LogToBoth(szLogString, ucPin);
      //Some pins are not tested and their pin mumbers are set to 99
      if (ucPin != 99) {
  int sValue= analogRead(ucPin);
  szLogString = "TestAllADCs():";
  LogToBoth(szLogString, sValue);
  float fVoltage = (sValue * 0.1875)/1000;
  szLogString = "TestAllADCs():V=";
  LogToBoth(szLogString, fVoltage);
  delay(100);
      } //if(ucPin!=99)
  } //for
  return;
} //TestAllADCs
//Last line
