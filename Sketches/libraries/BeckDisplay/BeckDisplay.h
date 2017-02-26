//BeckDisplay.h 2/26/17
#ifndef BECKDISPLAY_H
#define BECKDISPLAY_H
#include <BeckLib.h>
#include <Adafruit_SSD1306.h>

class BeckDisplay {
protected:
Adafruit_SSD1306*			pDisplay_;

public:
	BeckDisplay     (void);
	void		UpdateTanks(double adVolts);

};  //BeckDisplay
#endif  //BECKDISPLAY_H
