//BeckBlynk.h
#ifndef BECKBLYNK_H
#define BECKBLYNK_H
#include <BeckLib.h>

extern void BlynkLogLine(String szLogString);
extern void BlynkLogLine(String szLogString, String szLogValue);
extern void BlynkLogLine(String szLogString, int sLogValue);
extern void BlynkLogLine(String szLogString, float fLogValue);
extern void BlynkLogLine(String szLogString, double dValue);
extern void HandleBlynkLEDs();

class BeckBlynk {
protected:
  INT8    acBlynkAuthToken_[];
public:
          BeckBlynk       (const INT8 acBlynkAuthToken[]);
  void    Run             ();
};  //BeckBlynk
#endif  //BECKBLYNK_H
