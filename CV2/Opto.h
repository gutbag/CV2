#ifndef __CV2__Opto__
#define __CV2__Opto__

#include <Arduino.h>

class TriggeredOnOff;

class Opto
{
public:
	static Opto& instance(const uint8_t index);
	Opto(const uint8_t instanceIndex, const uint8_t aPin);
	virtual ~Opto();
	void setup();
	void loop(const unsigned long usNow);
private:
	void on();
	void off();

	uint8_t pin;
	uint8_t onState;
	TriggeredOnOff* onOff;
};

#endif /* defined(__CV2__Opto__) */
