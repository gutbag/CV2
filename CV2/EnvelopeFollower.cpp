#include "EnvelopeFollower.h"
#include "MIDI.h"
#include "Display.h"

EnvelopeFollower* EnvelopeFollower::theInstance = NULL;

EnvelopeFollower& EnvelopeFollower::instance()
{
	return *theInstance;
}

EnvelopeFollower::EnvelopeFollower(const int anEnvPin)
: OnOffTriggerable(0, ENV_FOLLOWER_TRIGGER_INSTANCE_CC),
  envPin(anEnvPin), value(0), lastSampleUs(0), envState(*this), gainCCValue(DO_NOT_SAVE_VALUE), gainMapMax(1023)
{
	theInstance = this;
	setDefaults(true, true);
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
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, 0, ENV_FOLLOWER_GAIN_CC);

	envState.setup();
}

void EnvelopeFollower::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	
	if (triggered)
	{
		if (usNow - lastSampleUs > SAMPLE_PERIOD_US)
		{
			value = map(analogRead(envPin), 0, 1023, 0, gainMapMax);
			if (value > 1023)
			{
				value = 1023;
				Display::instance().flashApostrophe(); // clipping
			}
			lastSampleUs = usNow;
			envState.loop(usNow);
		}
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
	switch (controllerNumber)
	{
		case ENV_FOLLOWER_GAIN_CC:
			gainCCValue = value;
			gainMapMax = 1023 + gainCCValue * 16; // effectively gain of x 3
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t EnvelopeFollower::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case ENV_FOLLOWER_GAIN_CC:
			return gainCCValue;
		default:
			return OnOffTriggerable::getControllerValue(controllerNumber);
	}
}

