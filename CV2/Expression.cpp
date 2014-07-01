#include "Expression.h"
#include "TriggeredOnOff.h"
#include "MIDI.h"

static Expression* pInstances[2] = {NULL, NULL};

Expression& Expression::instance(const uint8_t index)
{
	return *pInstances[index];
}

Expression::Expression(const uint8_t aPin, const uint8_t index, const uint8_t aMidiChannel)
: pin(aPin), value(0), enable(NULL), midiChannel(aMidiChannel), triggerInstanceCCValue(DO_NOT_SAVE_VALUE)
{
	pInstances[index] = this;
}

Expression::~Expression()
{
}

uint16_t Expression::getMinimum()
{
	return 0;
}

uint16_t Expression::getMaximum()
{
	return 1023;
}

uint16_t Expression::getValue()
{
//	Serial.print("Expression::getValue ");
//	Serial.println(value, DEC);

	return value;
}

void Expression::setup()
{
	MIDI::instance().setCCListener(this, midiChannel, EXPR_TRIGGER_INSTANCE_CC);

	value = analogRead(pin);
	
	// for now, if this is the first instance, attach the trigger
	// if the other Expr needs a trigger in future, pass the MID channel into the ctor
//	if (this == pInstances[0])
//	{
//		enable = &TriggeredOnOff::instance(EXPR_TRIGGER_MIDI_CHANNEL);
//	}
}

void Expression::loop(const unsigned long usNow)
{
	if (enable == NULL || (enable != NULL && enable->isOn()))
		value = analogRead(pin);
	else
		value = getMinimum();
}

void Expression::processCCMessage(const uint8_t channel,
						   const uint8_t controllerNumber,
						   const uint8_t value)
{
	Serial.print("Expression Message, ch: ");
	Serial.print(channel, DEC);
	Serial.print(" No: ");
	Serial.print(controllerNumber, DEC);
	Serial.print(" Val: ");
	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case EXPR_TRIGGER_INSTANCE_CC:
			triggerInstanceCCValue = value;
			if (triggerInstanceCCValue != 0)
			{
				enable = &TriggeredOnOff::instance(triggerInstanceCCValue - 1);
				enable->setDefaultOn(false); // default is off
			}
			else
			{
				enable = NULL;
			}
			break;
		default:
			break;
	}
}

uint8_t Expression::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case EXPR_TRIGGER_INSTANCE_CC:
			return triggerInstanceCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
			break;
	}
}

