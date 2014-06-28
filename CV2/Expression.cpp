#include "Expression.h"
#include "TriggeredOnOff.h"
#include "MIDI.h"

static Expression* pInstances[2] = {NULL, NULL};

Expression& Expression::instance(const uint8_t index)
{
	return *pInstances[index];
}

Expression::Expression(const uint8_t aPin, const uint8_t index)
: pin(aPin), value(0), enable(NULL)
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
	value = analogRead(pin);
	
	// for now, if this is the first instance, attach the trigger
	// if the other Expr needs a trigger in future, pass the MID channel into the ctor
	if (this == pInstances[0])
	{
		enable = &TriggeredOnOff::instance(EXPR_TRIGGER_MIDI_CHANNEL);
	}
}

void Expression::loop(const unsigned long usNow)
{
	if (enable != NULL && enable->isOn())
		value = analogRead(pin);
	else
		value = getMinimum();
}
