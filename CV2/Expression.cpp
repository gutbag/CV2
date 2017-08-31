#include "Expression.h"
#include "MIDI.h"

static Expression* pInstances[2] = {NULL, NULL};

Expression& Expression::instance(const uint8_t index)
{
	return *pInstances[index];
}

Expression::Expression(const uint8_t aPin, const uint8_t index, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, EXPR_TRIGGER_INSTANCE_CC),
  pin(aPin), value(0), midiChannel(aMidiChannel), invert(false)
{
	pInstances[index] = this;
	setDefaults(true, false);
	//setDefaults(true, true);
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
	OnOffTriggerable::setup();
	value = analogRead(pin);
	MIDI::instance().setCCListener(this, midiChannel, EXPR_INVERT_CC);
}

void Expression::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();

	if (triggered)
	{
		value = analogRead(pin);
		
		if (invert)
			value = getMaximum() - value;
	}
	else
	{
		value = getMinimum();
		
		if (invert)
			value = getMaximum();
	}
}

void Expression::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
	//	Serial.print("Expression Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case EXPR_INVERT_CC:
			invert = value == 1 ? true : false;
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t Expression::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case EXPR_INVERT_CC:
			return invert ? 1 : 0;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
}
