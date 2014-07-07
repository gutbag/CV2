#include "Bus.h"
#include "MIDI.h"
#include "LFO.h"
#include "Expression.h"
#include "EnvelopeFollower.h"
#include "Ramp.h"
#include "Noise.h"

static Bus* pInstances[4] = {NULL, NULL, NULL, NULL};

Bus& Bus::instance(const uint8_t index)
{
	return *pInstances[index];
}

Bus::Bus(const uint8_t index, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, BUS_TRIGGER_INSTANCE_CC),
  midiChannel(aMidiChannel),
  source1CCValue(0),
  source2CCValue(0),
  mixCCValue(63),
  pSource1(NULL),
  pSource2(NULL),
  value(0)
{
	pInstances[index] = this;
	setDefaults(true, false);
}

Bus::~Bus()
{
}

uint16_t Bus::getMinimum()
{
	return 0;
}

uint16_t Bus::getMaximum()
{
	return 511;
}

uint16_t Bus::getValue()
{
	return value;
}

void Bus::setup()
{
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, midiChannel, BUS_SOURCE_1_CC);
	MIDI::instance().setCCListener(this, midiChannel, BUS_SOURCE_2_CC);
	MIDI::instance().setCCListener(this, midiChannel, BUS_MIX_CC);
}

void Bus::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	
	if (triggered)
	{
		value = 0;
		
		if (pSource1 != NULL)
		{
			long max = map(mixCCValue, 0, 127, getMinimum(), getMaximum());
			value += map(pSource1->getValue(), pSource1->getMinimum(), pSource1->getMaximum(), getMinimum(), max);
		}
		
		if (pSource2 != NULL)
		{
			long max = map(127 - mixCCValue, 0, 127, getMinimum(), getMaximum());
			value += map(pSource2->getValue(), pSource2->getMinimum(), pSource2->getMaximum(), getMinimum(), max);
		}
	}
}

ValueProvider* Bus::getSource(const uint8_t sourceId)
{
	switch (sourceId)
	{
		case BUS_SOURCE_NONE_VALUE:
			return NULL;
		case BUS_SOURCE_LFO1_VALUE:
			return &LFO::instance(0);
		case BUS_SOURCE_LFO2_VALUE:
			return &LFO::instance(1);
		case BUS_SOURCE_EXPR1_VALUE:
			return &Expression::instance(0);
		case BUS_SOURCE_EXPR2_VALUE:
			return &Expression::instance(1);
		case BUS_SOURCE_ENV_VALUE:
			return &EnvelopeFollower::instance();
		case BUS_SOURCE_RAMP1_VALUE:
			return &Ramp::instance(0);
		case BUS_SOURCE_RAMP2_VALUE:
			return &Ramp::instance(1);
		case BUS_SOURCE_NOISE1_VALUE:
			return &Noise::instance(0);
		case BUS_SOURCE_NOISE2_VALUE:
			return &Noise::instance(1);
		default:
			return NULL;
	}
}

void Bus::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
	//	Serial.print("Bus Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case BUS_SOURCE_1_CC:
			source1CCValue = value;
			pSource1 = getSource(source1CCValue);
			break;
		case BUS_SOURCE_2_CC:
			source2CCValue = value;
			pSource2 = getSource(source2CCValue);
			break;
		case BUS_MIX_CC:
			mixCCValue = value;
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t Bus::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case BUS_SOURCE_1_CC:
			return source1CCValue;
		case BUS_SOURCE_2_CC:
			return source2CCValue;
		case BUS_MIX_CC:
			return mixCCValue;
		default:
			return OnOffTriggerable::getControllerValue(controllerNumber);
	}
}
