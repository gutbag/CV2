#include "EnvelopeState.h"
#include "EnvelopeFollower.h"
#include "MIDI.h"
#include "MIDIMessages.h"

/**
 * An OnOffStateProvider based on the envelope level compared to two thresholds,
 * one for the low to high transition, the other for high to low.
 */

EnvelopeState::EnvelopeState(EnvelopeFollower& anEnv)
: OnOffTriggerable(0, ENV_STATE_TRIGGER_INSTANCE_CC),
  env(anEnv), lowHighThreshold(511), highLowThreshold(511), sensitivity(10),
  changeStateCount(0), onState(false)
{
	setDefaults(true, true);
}

EnvelopeState::~EnvelopeState()
{
}

void EnvelopeState::setup()
{
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, 0, ENV_STATE_LOW_HIGH_THRESHOLD_CC);
	MIDI::instance().setCCListener(this, 0, ENV_STATE_HIGH_LOW_THRESHOLD_CC);
	MIDI::instance().setCCListener(this, 0, ENV_STATE_THRESHOLD_SENSITIVITY_CC);
}

void EnvelopeState::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	
	if (triggered)
	{
		uint16_t value = env.getValue();
		
		if (onState)
		{
			if (value <= highLowThreshold)
			{
				changeStateCount++;
				
				if (changeStateCount >= sensitivity)
				{
					onState = false;
					changeStateCount = 0;
				}
			}
			else
				changeStateCount = 0;
		}
		else
		{
			if (value >= lowHighThreshold)
			{
				changeStateCount++;
				
				if (changeStateCount >= sensitivity)
				{
					onState = true;
					changeStateCount = 0;
				}
			}
			else
				changeStateCount = 0;
		}
	}
	else
	{
		onState = false;
		changeStateCount = 0;
	}
}

/*
 #define DEBOUNCE_US (1000 * 100)
void EnvelopeState::loop(const unsigned long usNow)
{
	enable.loop(usNow);
	
	if (enable.isOn())
	{
		uint16_t value = env.getValue();
		boolean newOnState = false;
		
		if (onState)
		{
			if (value <= highLowThreshold)
				newOnState = false;
		}
		else
		{
			if (value >= lowHighThreshold)
				newOnState = true;
		}
		
		if (newOnState != lastOnState)
		{
			stableSinceUs = usNow;
		}
		else if (usNow - stableSinceUs >= DEBOUNCE_US)
		{
			onState = newOnState;
		}
		
		lastOnState = newOnState;
	}
}
*/
void EnvelopeState::processCCMessage(const uint8_t channel,
							  const uint8_t controllerNumber,
							  const uint8_t value)
{
//	Serial.print("EnvelopeState Message, ch: ");
//	Serial.print(channel, DEC);
//	Serial.print(" No: ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" Val: ");
//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case ENV_STATE_LOW_HIGH_THRESHOLD_CC:
			lowHighThreshold = value << 3;
			if (0)
			{
				Serial.print("0-1 th ");
				Serial.println(lowHighThreshold, DEC);
			}
			break;
		case ENV_STATE_HIGH_LOW_THRESHOLD_CC:
			highLowThreshold = value << 3; // TODO: better not to assume env range is 1024
			break;
		case ENV_STATE_THRESHOLD_SENSITIVITY_CC:
			sensitivity = value;
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t EnvelopeState::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case ENV_STATE_LOW_HIGH_THRESHOLD_CC:
			return lowHighThreshold >> 3;
		case ENV_STATE_HIGH_LOW_THRESHOLD_CC:
			return highLowThreshold >> 3;
		case ENV_STATE_THRESHOLD_SENSITIVITY_CC:
			return sensitivity;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
}

boolean EnvelopeState::isOn()
{
	return onState;
}

/*
boolean EnvelopeState::isOn()
{
	if (enable.isOn())
	{
		uint16_t value = env.getValue();
		
		if (onState)
		{
			if (value <= highLowThreshold)
			{
				changeStateCount++;
				
				if (changeStateCount >= sensitivity)
				{
					onState = false;
					changeStateCount = 0;
				}
			}
			else
				changeStateCount = 0;
		}
		else
		{
			if (value >= lowHighThreshold)
			{
				changeStateCount++;
				
				if (changeStateCount >= sensitivity)
				{
					onState = true;
					changeStateCount = 0;
				}
			}
			else
				changeStateCount = 0;
		}
	}
	
	return onState;
}
*/
