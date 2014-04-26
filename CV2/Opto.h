#ifndef __CV2__Opto__
#define __CV2__Opto__

#include <Arduino.h>

class Opto
{
public:
	Opto(const uint8_t aPin);
	virtual ~Opto();
	void setup();
	void on();
	void off();
private:
	uint8_t pin;
};

#endif /* defined(__CV2__Opto__) */
