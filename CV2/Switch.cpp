#include "Switch.h"

static const unsigned long DEBOUNCE_US = 20000;

Switch::Switch(const uint8_t aPin)
: pin(aPin), on(false), stableSinceUs(0), lastPinState(LOW)
{
	
}

Switch::~Switch()
{
	
}

void Switch::setup()
{
	pinMode(pin, INPUT_PULLUP);
	lastPinState = digitalRead(pin);
}

void Switch::loop(const unsigned long usNow)
{
	int newPinState = digitalRead(pin);
	
	if (newPinState != lastPinState)
	{
		stableSinceUs = usNow;
	}
	else if (usNow - stableSinceUs >= DEBOUNCE_US)
	{
		on = newPinState == LOW ? true : false;
	}
				
	lastPinState = newPinState;
}

boolean Switch::isOn()
{
	return on;
}
