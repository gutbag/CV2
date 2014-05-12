#include "Patch.h"

Patch::Patch(const uint8_t aDownPin, const uint8_t anUpPin)
: downSwitch(aDownPin), upSwitch(anUpPin)
{
	
}

Patch::~Patch()
{
	
}

void Patch::processPCMessage(const uint8_t channel, const uint8_t programNumber)
{
	
}

void Patch::setup()
{
	downSwitch.setup();
	upSwitch.setup();
}

void Patch::loop(const unsigned long usNow)
{
	downSwitch.loop(usNow);
	upSwitch.loop(usNow);
}
