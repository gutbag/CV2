#include "LFO.h"

LFO::LFO()
{
	
}

LFO::~LFO()
{
	
}

uint16_t LFO::getMinimum()
{
	return 0;
}

uint16_t LFO::getMaximum()
{
	return 255;
}

uint16_t LFO::getValue()
{
	return 0;
}

void LFO::setup()
{
	Serial1.begin(31250);
}

void LFO::loop(const unsigned long usNow)
{
}
