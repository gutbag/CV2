#include "Footswitch.h"

Footswitch::Footswitch(const uint8_t aSwPin, const uint8_t anLedPin)
: swPin(aSwPin), ledPin(anLedPin)
{
	
}

Footswitch::~Footswitch()
{
	
}

void Footswitch::setup()
{
	pinMode(swPin, INPUT_PULLUP);
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
}

void Footswitch::loop(const unsigned long usNow)
{
	
}
