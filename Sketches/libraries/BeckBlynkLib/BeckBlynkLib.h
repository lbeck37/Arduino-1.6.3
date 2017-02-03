//BeckBlynkLib.h
#ifndef BECKBLYNKLIB_H
#define BECKBLYNKLIB_H
#include <BeckLib.h>

extern void RunBlynk();
extern void ConfigBlynk(const char acBlynkAuthToken[]);
extern void ConfigBlynk(const char acBlynkAuthToken[], IPAddress oIPAddress);

extern void BlynkLogLine(String szLogString);
extern void BlynkLogLine(String szLogString, String szLogValue);
extern void BlynkLogLine(String szLogString, int sLogValue);
extern void BlynkLogLine(String szLogString, float fLogValue);
extern void BlynkLogLine(String szLogString, double dValue);
extern void HandleBlynkLEDs();

class BeckBlynk {
	BeckBlynk();
	void 		Config					(const char acBlynkAuthToken[]);
	void 		Config					(const char acBlynkAuthToken[], IPAddress oIPAddress);
	void		Run							();
};	//BeckBlynk

#endif	//BECKBLYNKLIB_H
