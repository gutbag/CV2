#ifndef __CV2__Freeze__
#define __CV2__Freeze__

#include <Arduino.h>
#include "OnOffTriggerable.h"

class Freeze : public OnOffTriggerable
{
public:
	Freeze(const uint8_t aPin, const uint8_t aMidiChannel);
	virtual ~Freeze();
	void setup();
	void loop(const unsigned long usNow);
private:
	void on();
	void off();
	
	uint8_t pin;
	uint8_t midiChannel;
	uint8_t onState;
};

#endif /* defined(__CV2__Freeze__) */
