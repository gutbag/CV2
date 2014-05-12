#include "Footswitch.h"

Footswitch::Footswitch(const uint8_t aSwPin, const uint8_t anLedPin)
: Switch(aSwPin), ledPin(anLedPin)
{
	
}

Footswitch::~Footswitch()
{
	
}

void Footswitch::setup()
{
	Switch::setup();
	
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
}

void Footswitch::loop(const unsigned long usNow)
{
	Switch::loop(usNow);
}
