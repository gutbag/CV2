#include "EnvelopeFollower.h"
#include "MIDI.h"

EnvelopeFollower* EnvelopeFollower::theInstance = NULL;

EnvelopeFollower& EnvelopeFollower::instance()
{
	return *theInstance;
}

EnvelopeFollower::EnvelopeFollower(const int anEnvPin)
: envPin(anEnvPin), value(0), lastSampleUs(0), envState(*this)
{
	theInstance = this;
}

EnvelopeFollower::~EnvelopeFollower()
{
	
}

EnvelopeState& EnvelopeFollower::getEnvelopeState()
{
	return envState;
}

void EnvelopeFollower::setup()
{
	envState.setup();
}

void EnvelopeFollower::loop(const unsigned long usNow)
{
	if (usNow - lastSampleUs > SAMPLE_PERIOD_US)
	{
		value = analogRead(envPin);
		lastSampleUs = usNow;
		envState.loop(usNow);
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

