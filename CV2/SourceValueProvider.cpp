#include "SourceValueProvider.h"
#include "MIDI.h"
#include "LFO.h"
#include "Expression.h"
#include "EnvelopeFollower.h"
#include "Ramp.h"
#include "Noise.h"
#include "Bus.h"
#include "AWG.h"

SourceValueProvider::SourceValueProvider(const uint8_t aMidiChannel, const uint8_t aMinCC, const uint8_t aMaxCC, const uint8_t aSourceCC)
: midiChannel(aMidiChannel), minCC(aMinCC), maxCC(aMaxCC), sourceCC(aSourceCC), pSource(NULL), sourceId(DO_NOT_SAVE_VALUE)
{
}

SourceValueProvider::SourceValueProvider()
: midiChannel(~0), minCC(~0), maxCC(~0), sourceCC(~0), pSource(NULL), sourceId(DO_NOT_SAVE_VALUE)
{
}

SourceValueProvider::~SourceValueProvider()
{
}

void SourceValueProvider::setup()
{
	MIDI::instance().setCCListener(this, midiChannel, minCC);
	MIDI::instance().setCCListener(this, midiChannel, maxCC);
	MIDI::instance().setCCListener(this, midiChannel, sourceCC);
}

void SourceValueProvider::processCCMessage(const uint8_t channel,
					  const uint8_t controllerNumber,
					  const uint8_t value)
{
//	Serial.print("SourceValueProvider Message, ch: ");
//	Serial.print(channel, DEC);
//	Serial.print(" No: ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" Val: ");
//	Serial.println(value, DEC);
	
	if (controllerNumber == minCC)
	{
		minValue = value;
	}
	else if (controllerNumber == maxCC)
	{
		maxValue = value;
	}
	else if (controllerNumber == sourceCC)
	{
		sourceId = value;
		
		switch (sourceId)
		{
			case SOURCE_VALUE_PROVIDER_SOURCE_NONE_VALUE:
				pSource = NULL;
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_LFO1_VALUE:
				pSource = &LFO::instance(0);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_LFO2_VALUE:
				pSource = &LFO::instance(1);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_EXPR1_VALUE:
				pSource = &Expression::instance(0);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_EXPR2_VALUE:
				pSource = &Expression::instance(1);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_ENV_VALUE:
				pSource = &EnvelopeFollower::instance();
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_NOISE1_VALUE:
				pSource = &Noise::instance(0);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_NOISE2_VALUE:
				pSource = &Noise::instance(1);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_RAMP1_VALUE:
				pSource = &Ramp::instance(0);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_RAMP2_VALUE:
				pSource = &Ramp::instance(1);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_BUS1_VALUE:
				pSource = &Bus::instance(0);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_BUS2_VALUE:
				pSource = &Bus::instance(1);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_BUS3_VALUE:
				pSource = &Bus::instance(2);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_BUS4_VALUE:
				pSource = &Bus::instance(3);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_AWG1_VALUE:
				pSource = &AWG::instance(0);
				break;
			case SOURCE_VALUE_PROVIDER_SOURCE_AWG2_VALUE:
				pSource = &AWG::instance(1);
				break;
			default:
				break;
		}
	}
}

uint8_t SourceValueProvider::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	if (controllerNumber == minCC)
		return minValue;
	else if (controllerNumber == maxCC)
		return maxValue;
	else if (controllerNumber == sourceCC)
		return sourceId;
	else
		return DO_NOT_SAVE_VALUE;
}

uint16_t SourceValueProvider::getMinimum()
{
	return minValue;
}

void SourceValueProvider::setMinimum(const uint8_t value)
{
	minValue = value;
}

void SourceValueProvider::setMaximum(const uint8_t value)
{
	maxValue = value;
}

uint16_t SourceValueProvider::getMaximum()
{
	return maxValue;
}

uint16_t SourceValueProvider::getValue()
{
	if (pSource != NULL)
	{
		uint16_t sourceValue = pSource->getValue();
		uint16_t sourceMin = pSource->getMinimum();
		uint16_t sourceMax = pSource->getMaximum();
		
		if (sourceMin == minValue && sourceMax == maxValue) // range is the same, no need to map
			return sourceValue;
		else
			return map(sourceValue, sourceMin, sourceMax, minValue, maxValue);
	}
	else
		return minValue;
}
