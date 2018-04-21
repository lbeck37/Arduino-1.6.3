#include <Arduino.h>
#include <HX711.h>
#include <Streaming.h>

/* Beck 4/20/18
#if ARDUINO_VERSION <= 106
    // "yield" is not implemented as noop in older Arduino Core releases, so let's define it.
    // See also: https://stackoverflow.com/questions/34497758/what-is-the-secret-of-the-arduino-yieldfunction/34498165#34498165
    void yield(void) {};
#endif
*/

HX711::HX711(byte dout, byte pd_sck, byte gain) {
	begin(dout, pd_sck, gain);
}

HX711::HX711() {
}

HX711::~HX711() {
}

void HX711::begin(byte dout, byte pd_sck, byte gain) {
	PD_SCK = pd_sck;
	DOUT = dout;

	Serial << "HX711::begin(): Call pinMode(" << PD_SCK << ", OUTPUT)" << endl;
	pinMode(PD_SCK, OUTPUT);

	Serial << "HX711::begin(): Call pinMode(" << DOUT << ", INPUT)" << endl;
	pinMode(DOUT, INPUT);

  Serial.println("HX711::begin(): Call set_gain(gain)");
	set_gain(gain);
  Serial.println("HX711::begin(): Done");
	return;
}	//begin


bool HX711::is_ready() {
	return digitalRead(DOUT) == LOW;
}	//is_ready


void HX711::set_gain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
		default:
			Serial << "HX711::set_gain(): Bad case in switch" << endl;
			GAIN = 1;
			break;
	}	//switch

	Serial << "HX711::set_gain(): Call digitalWrite(PD_SCK, LOW)" << endl;
	digitalWrite(PD_SCK, LOW);
	Serial << "HX711::set_gain(): Call read()" << endl;
	read();
	return;
}	//set_gain


long HX711::read() {
	unsigned long ulValue = 0;
	uint8_t aucData[3] = { 0 };
	uint8_t ucFiller = 0x00;

	// wait for the chip to become ready
	while (!is_ready()) {
		// Will do nothing on Arduino but prevent resets of ESP8266 (Watchdog Issue)
		yield();
	}	//while

	// pulse the clock pin 24 times to read the data
	aucData[2] = shiftIn(DOUT, PD_SCK, MSBFIRST);
	aucData[1] = shiftIn(DOUT, PD_SCK, MSBFIRST);
	aucData[0] = shiftIn(DOUT, PD_SCK, MSBFIRST);

	// set the channel and the gain factor for the next reading using the clock pin
	for (unsigned int i = 0; i < GAIN; i++) {
		digitalWrite(PD_SCK, HIGH);
		digitalWrite(PD_SCK, LOW);
	}	//for

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if (aucData[2] & 0x80) {
		ucFiller = 0xFF;
	} else {
		ucFiller = 0x00;
	}

	// Construct a 32-bit signed integer
	ulValue = ( static_cast<unsigned long>(ucFiller) << 24
			| static_cast<unsigned long>(aucData[2]) << 16
			| static_cast<unsigned long>(aucData[1]) << 8
			| static_cast<unsigned long>(aucData[0]) );

	Serial << "HX711::read(): Returning ulValue= " << ulValue << endl;
	return static_cast<long>(ulValue);
}	//read


long HX711::read_average(byte times) {
	long sum = 0;
	for (byte i = 0; i < times; i++) {
		sum += read();
		yield();
	}
	return sum / times;
}	//read_average


double HX711::get_value(byte times) {
	return read_average(times) - OFFSET;
}	//get_value


float HX711::get_units(byte times) {
	return get_value(times) / SCALE;
}	//get_units


void HX711::tare(byte times) {
	double sum = read_average(times);
	set_offset(sum);
	return;
}	//tare


void HX711::set_scale(float scale) {
	SCALE = scale;
	return;
}	//set_scale


float HX711::get_scale() {
	return SCALE;
}	//get_scale


void HX711::set_offset(long offset) {
	OFFSET = offset;
	return;
}	//set_offset


long HX711::get_offset() {
	return OFFSET;
}	//get_offset


void HX711::power_down() {
	digitalWrite(PD_SCK, LOW);
	digitalWrite(PD_SCK, HIGH);
	return;
}	//power_down


void HX711::power_up() {
	digitalWrite(PD_SCK, LOW);
	return;
}	//power_up
//Lastline.
