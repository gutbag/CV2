#include "Switch.h"

Switch::Switch(const uint8_t aPin)
{
	
}

Switch::~Switch()
{
	
}

void Switch::setup()
{
	pinMode(pin, INPUT_PULLUP);
}

void Switch::loop(const unsigned long usNow)
{
	// read switch state
	// track state
	// report trigger condition if appropriate
}
