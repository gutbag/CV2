#include "MovingAverage.h"

MovingAverage::MovingAverage()
: depth(1)
{
	for (uint8_t i=0; i<MAX_DEPTH; i++)
		values[i] = 0;
}

MovingAverage::~MovingAverage()
{
}

void MovingAverage::addValue(const uint8_t n)
{
	
}

uint8_t MovingAverage::getAverage()
{
	
}

void MovingAverage::setDepth(const uint8_t n)
{
	if (n <= MAX_DEPTH)
		depth = n;
}
