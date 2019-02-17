//BeckLogLib.cpp, 1/26/19
#include <BeckLogLib.h>
#include <BeckBlynkLib.h>

//Global variables
long            lLineCount      = 0;      //Serial Monitor uses for clarity.
String          szLogLine;
bool            bDebugLog       = true;   //Used to limit number of printouts.


void LogJustToSerial(String sLogline){
  String sFullLogline=LOG0 + sLogline;
  Serial << sFullLogline << endl;
  return;
} //LogJustToSerial


String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szHeader += szGetTime(millis());
  return szHeader;
} //szLogLineHeader


//szAddLeadingZeros() adds 1 or 2 zeros (depending on sNumDigits being 3 or not).
String szAddZeros(int sValue, int sNumDigits){
  String szReturn;
  if ((sNumDigits == 3) && (sValue < 100)){
    szReturn= "0";
  } //if((sNumDigits==3)&&(sValue<100)
  if (sValue < 10){
    szReturn += "0";
  } //if(lValue<10)
  szReturn += String(sValue);
  return szReturn;
} //szAddZeros


//LogToBoth() and BlynkLogLine()have multiple versions
//depending on there being a 2nd variable and its type.
void LogToBoth(String szLogString){
  Serial << LOG0 << szLogString << endl;
  BlynkLogLine(szLogString);
  return;
} //LogToBoth:empty


void LogToBoth(String szLogString, String szLogValue){
  Serial << LOG0 << szLogString << " " << szLogValue << endl;
  BlynkLogLine(szLogString, szLogValue);
  return;
} //LogToBoth:String


void LogToBoth(String szLogString, int sLogValue){
  Serial << LOG0 << szLogString << " " << sLogValue << endl;
  BlynkLogLine(szLogString, sLogValue);
  return;
} //LogToBoth:int


void LogToBoth(String szLogString, UINT32 uwLogValue){
  Serial << LOG0 << szLogString << " " << uwLogValue << endl;
  return;
} //LogToBoth:UINT32


void LogToBoth(String szLogString, float fLogValue){
  Serial << LOG0 << szLogString << " " << fLogValue << endl;
  BlynkLogLine(szLogString, fLogValue);
  return;
} //LogToBoth:float


void LogToBoth(String szLogString, double dLogValue){
  Serial << LOG0 << szLogString << " " << dLogValue << endl;
  BlynkLogLine(szLogString, dLogValue);
  return;
} //LogToBoth:double

void LogToSerial(String szLogString){
  Serial << LOG0 << szLogString << endl;
  return;
} //LogToSerial:empty


void LogToSerial(String szLogString, String szLogValue){
  Serial << LOG0 << szLogString << " " << szLogValue << endl;
  return;
} //LogToSerial:String


void LogToSerial(String szLogString, int sLogValue){
  Serial << LOG0 << szLogString << " " << sLogValue << endl;
  return;
} //LogToSerial:int


void LogToSerial(String szLogString, int sValue1, int sValue2, int sValue3){
  //Created for logging 3 acceleration axis
  Serial << LOG0 << szLogString << " " << sValue1 << "   " << sValue2 << "   " << sValue3 << "   " << endl;
  return;
} //LogToSerial:int


void LogToSerial(String szLogString, UINT16 usLogValue){
  Serial << LOG0 << szLogString << " " << usLogValue << endl;
  return;
} //LogToSerial:UINT16


void LogToSerial(String szLogString, UINT32 uwLogValue){
  Serial << LOG0 << szLogString << " " << uwLogValue << endl;
  return;
} //LogToSerial:UINT32


void LogToSerial(String szLogString, float fLogValue){
  Serial << LOG0 << szLogString << " " << fLogValue << endl;
  return;
} //LogToSerial:float


void LogToSerial(String szLogString, double dLogValue){
  Serial << LOG0 << szLogString << " " << dLogValue << endl;
  return;
} //LogToSerial:double
//Last line.
