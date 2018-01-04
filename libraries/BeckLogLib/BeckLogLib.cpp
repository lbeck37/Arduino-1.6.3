//BeckLogLib.cpp, Aug 3, 2017
#include <BeckLogLib.h>

//Global variables
long                  lLineCount      = 0;  //Serial Monitor uses for clarity.
String                szLogLine;

bool            bDebugLog= true;   //Used to limit number of printouts.


void LogJustToSerial(String sLogline){
  String sFullLogline=LOG0 + sLogline;
  Serial << sFullLogline << endl;
  return;
} //LogJustToSerial


/*
String szLogLineHeader(long lLineCount){
  String szHeader= "";
  szHeader += lLineCount;
  szHeader += " ";
  //szHeader += szGetTime(millis());
  return szHeader;
} //szLogLineHeader
*/


/*
String szGetTime(long lMsec){
  String  szString;
  int sDays    =    lMsec                                               / lMsecPerDay ;
  int sHours   =   (lMsec % lMsecPerDay)                                / lMsecPerHour;
  int sMinutes =  ((lMsec % lMsecPerDay) % lMsecPerHour)                / lMsecPerMin ;
  int sSeconds = (((lMsec % lMsecPerDay) % lMsecPerHour) % lMsecPerMin) / lMsecPerSec;
  int sMsec    =    lMsec % lMsecPerSec;
  szString = String(sDays) + ":";
  szString+= String(szAddZeros(sHours, 2)) + ":";
  szString+= String(szAddZeros(sMinutes, 2)) + ":";
  szString+= String(szAddZeros(sSeconds, 2)) + ".";
  szString+= String(szAddZeros(sMsec, 3)) + " ";  //Trailing blank.
  return szString;
} //szGetTime
*/


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


/*
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
*/
//Last line.
