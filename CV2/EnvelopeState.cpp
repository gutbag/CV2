#include "EnvelopeState.h"
#include "EnvelopeFollower.h"
#include "Footswitch.h"
#include "MIDI.h"
#include "MIDIMessages.h"

/**
 * An OnOffStateProvider based on the envelope level compared to two thresholds,
 * one for the low to high transition, the other for high to low.
 */

EnvelopeState::EnvelopeState(EnvelopeFollower& anEnv)
: env(anEnv), lowHighThreshold(511), highLowThreshold(511), sensitivity(10), changeStateCount(0), onState(false), triggerModeCCValue(DO_NOT_SAVE_VALUE), controlTypeCCValue(DO_NOT_SAVE_VALUE) /*, stableSinceUs(0), lastOnState(false)*/
{
}

EnvelopeState::~EnvelopeState()
{
}

void EnvelopeState::setup()
{
	MIDI::instance().setCCListener(this, 0, ENV_STATE_ENABLE_TRIGGER_MODE_CC);
	MIDI::instance().setCCListener(this, 0, ENV_STATE_ENABLE_CONTROL_TYPE_CC);
	MIDI::instance().setCCListener(this, 0, ENV_STATE_LOW_HIGH_THRESHOLD_CC);
	MIDI::instance().setCCListener(this, 0, ENV_STATE_HIGH_LOW_THRESHOLD_CC);
	MIDI::instance().setCCListener(this, 0, ENV_STATE_THRESHOLD_SENSITIVITY_CC);
}

void EnvelopeState::loop(const unsigned long usNow)
{
	enable.loop(usNow);
	
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
		case ENV_STATE_ENABLE_TRIGGER_MODE_CC:
			triggerModeCCValue = value;
			switch (triggerModeCCValue)
			{
			case ENV_STATE_ENABLE_TRIGGER_MODE_LEVEL_NON_INV_VALUE:
				enable.setMode(TriggeredOnOff::LEVEL_NON_INV);
				break;
			case ENV_STATE_ENABLE_TRIGGER_MODE_LEVEL_INV_VALUE:
				enable.setMode(TriggeredOnOff::LEVEL_INV);
				break;
			case ENV_STATE_ENABLE_TRIGGER_MODE_EDGE_OFF_ON_VALUE:
				enable.setMode(TriggeredOnOff::EDGE_OFF_ON);
				break;
			case ENV_STATE_ENABLE_TRIGGER_MODE_EDGE_ON_OFF_VALUE:
				enable.setMode(TriggeredOnOff::EDGE_ON_OFF);
				break;
			default:
				break;
			}
			break;
		case ENV_STATE_ENABLE_CONTROL_TYPE_CC:
			controlTypeCCValue = value;
			switch (controlTypeCCValue)
			{
			case ENV_STATE_ENABLE_CONTROL_NONE_VALUE:
				enable.setProvider(NULL);
				enable.setOn(true);
				break;
			case ENV_STATE_ENABLE_CONTROL_FSW_1_VALUE:
				enable.setProvider(&Footswitch::instance(0));
				break;
			case ENV_STATE_ENABLE_CONTROL_FSW_2_VALUE:
				enable.setProvider(&Footswitch::instance(1));
				break;
			case ENV_STATE_ENABLE_CONTROL_FSW_3_VALUE:
				enable.setProvider(&Footswitch::instance(2));
				break;
			default:
				break;
			}
			break;
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
			break;
	}
}

uint8_t EnvelopeState::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case ENV_STATE_ENABLE_TRIGGER_MODE_CC:
			return triggerModeCCValue;
		case ENV_STATE_ENABLE_CONTROL_TYPE_CC:
			return controlTypeCCValue;
		case ENV_STATE_LOW_HIGH_THRESHOLD_CC:
			return lowHighThreshold >> 2;
		case ENV_STATE_HIGH_LOW_THRESHOLD_CC:
			return highLowThreshold >> 2;
		case ENV_STATE_THRESHOLD_SENSITIVITY_CC:
			return sensitivity;
		default:
			return DO_NOT_SAVE_VALUE;
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
