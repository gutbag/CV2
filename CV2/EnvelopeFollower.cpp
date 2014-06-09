#include "EnvelopeFollower.h"
#include "MIDI.h"

EnvelopeFollower* EnvelopeFollower::theInstance = NULL;

EnvelopeFollower& EnvelopeFollower::instance()
{
	return *theInstance;
}

EnvelopeFollower::EnvelopeFollower(const int anEnvPin)
: envPin(anEnvPin), value(0), lastSampleUs(0)
{
	theInstance = this;
}

EnvelopeFollower::~EnvelopeFollower()
{
	
}

void EnvelopeFollower::setup()
{
	
}

void EnvelopeFollower::loop(const unsigned long usNow)
{
	if (usNow - lastSampleUs > SAMPLE_PERIOD_US)
	{
		value = analogRead(envPin);
		lastSampleUs = usNow;
	}
}

uint16_t EnvelopeFollower::getMinimum()
{
	return 0;
}

uint16_t EnvelopeFollower::getMaximum()
{
	return 1023;
}

uint16_t EnvelopeFollower::getValue()
{
	return value;
}

void EnvelopeFollower::processCCMessage(const uint8_t channel,
					  const uint8_t controllerNumber,
					  const uint8_t value)
{
	
}

uint8_t EnvelopeFollower::getControllerValue(const uint8_t controllerNumber)
{
	return DO_NOT_SAVE_VALUE;
}

