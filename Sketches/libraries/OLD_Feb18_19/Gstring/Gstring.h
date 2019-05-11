#ifndef GSTRING_H
#define GSTRING_H

#include "Arduino.h"

class Gstring : public Print {
public:
   Gstring();
   ~Gstring();
   virtual void write(uint8_t usChar);
   void blink(int time);
   void on();
   void off();
};

#endif