#include "Expression.h"

static Expression* pInstances[2] = {NULL, NULL};

Expression& Expression::instance(const uint8_t index)
{
	return *pInstances[index];
}

Expression::Expression(const uint8_t aPin, const uint8_t index)
: pin(aPin), value(0)
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
}

void Expression::loop(const unsigned long usNow)
{
	value = analogRead(pin);
}
