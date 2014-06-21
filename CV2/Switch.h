#ifndef __CV2__Switch__
#define __CV2__Switch__

#include <Arduino.h>
#include "OnOffStateProvider.h"

class Switch : public OnOffStateProvider
{
public:
	Switch(const uint8_t aPin);
	virtual ~Switch();
	void setup();
	void loop(const unsigned long usNow);
	boolean isOn();
private:
	uint8_t pin;
	boolean on;
	unsigned long stableSinceUs;
	int lastPinState;
};

#endif /* defined(__CV2__Switch__) */
