#include "CVOutput.h"
#include "DAC.h"

CVOutput::CVOutput(DAC& aDac, const uint8_t anOutput)
: dac(aDac),
  output(anOutput),
  value(0),
  minimum(0),
  maximum(255),
  pProvider(NULL),
  lastProviderValue(~0)
{
}

CVOutput::~CVOutput()
{
}

void CVOutput::setup()
{
	dac.setOutput(output, value);
}

void CVOutput::setValueProvider(ValueProvider* aValueProvider)
{
	pProvider = aValueProvider;
}

void CVOutput::loop(const unsigned int usNow)
{
	if (pProvider != NULL)
	{
		uint16_t pValue = pProvider->getValue();
		
		if (pValue != lastProviderValue) // TODO: not enough - min and max might have changed!!!
		{
			// TODO: get value and scale
			// if resulting value different from H/W, set it
			lastProviderValue = pValue;
		}
	}
	else // use minimum value if it's changed
	{
		
	}
}

void CVOutput::setMinimum(const uint8_t value)
{
	minimum = value;
}

void CVOutput::setMaximum(const uint8_t value)
{
	maximum = value;
}
