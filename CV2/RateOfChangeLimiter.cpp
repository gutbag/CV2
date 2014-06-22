#include "RateOfChangeLimiter.h"

RateOfChangeLimiter::RateOfChangeLimiter()
: target(0), value(0), numSteps(0), stepValue(1)
{
	
}

RateOfChangeLimiter::~RateOfChangeLimiter()
{
}

uint8_t RateOfChangeLimiter::getValue()
{
	value += stepValue;
	
	if (stepValue < 0)
	{
		if (value < target)
			value = target;
	}
	else
	{
		if (value > target)
			value = target;
	}
	
	return value;
}

void RateOfChangeLimiter::setTarget(const uint8_t n)
{
	target = n;
	stepValue = (target - value) / numSteps;
	if (stepValue == 0)
		stepValue = 1;
}

void RateOfChangeLimiter::setSteps(const uint8_t n)
{
	if (n > 0)
		numSteps = n;
	else
		numSteps = 1;
}
