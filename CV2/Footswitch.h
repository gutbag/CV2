#ifndef __CV2__Footswitch__
#define __CV2__Footswitch__

#include <Arduino.h>
#include "Switch.h"

class Footswitch : public Switch
{
public:
	static Footswitch& instance(const uint8_t index);
	Footswitch(const uint8_t instanceIndex, const uint8_t aSwPin, const uint8_t anLedPin);
	virtual ~Footswitch();
	void setup();
	void loop(const unsigned long usNow);
private:
	uint8_t ledPin;
};

#endif /* defined(__CV2__Footswitch__) */
