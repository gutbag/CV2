#ifndef __CV2__Footswitch__
#define __CV2__Footswitch__

#include <Arduino.h>

class Footswitch
{
public:
	Footswitch(const uint8_t aSwPin, const uint8_t anLedPin);
	virtual ~Footswitch();
	void setup();
	void loop(const unsigned long usNow);
private:
	uint8_t swPin;
	uint8_t ledPin;
};

#endif /* defined(__CV2__Footswitch__) */
