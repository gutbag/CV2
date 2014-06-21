#include "Footswitch.h"

static Footswitch* pInstances[3] = {NULL, NULL, NULL};

Footswitch& Footswitch::instance(const uint8_t index)
{
	return *pInstances[index];
}

Footswitch::Footswitch(const uint8_t instanceIndex, const uint8_t aSwPin, const uint8_t anLedPin)
: Switch(aSwPin), ledPin(anLedPin)
{
	pInstances[instanceIndex] = this;
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
