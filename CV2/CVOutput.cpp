#include "CVOutput.h"
#include "DAC.h"
#include "ValueProvider.h"
#include "MIDI.h"

CVOutput::CVOutput(DAC& aDac, const uint8_t anOutput)
: dac(aDac),
  output(anOutput),
  value(0),
  minimum(0),
  maximum(255),
  pProvider(NULL),
  lastProviderValue(~0),
  dirty(false)
{
}

CVOutput::~CVOutput()
{
}

void CVOutput::setup()
{
	dac.setOutput(output, value);
	
	// TODO: can't use output alone - will clash with 9V CVs
	MIDI::instance().setListener(this, output, 0xb0, 0);
	MIDI::instance().setListener(this, output, 0xb0, 1);
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
		if (dirty)
		{
//			Serial.print("output: ");
//			Serial.println(output, DEC);
//			Serial.print("loop setting: ");
//			Serial.println(minimum, HEX);
			dac.setOutput(output, minimum);
			dirty = false;
			//dac.setOutput(0, minimum);
		}
	}
}

void CVOutput::setMinimum(const uint8_t value)
{
	minimum = value;
	dirty = true;
//	Serial.print("setMinimum: ");
//	Serial.println(minimum, HEX);
}

void CVOutput::setMaximum(const uint8_t value)
{
	maximum = value;
	dirty = true;
//	Serial.print("setMaximum: ");
//	Serial.println(maximum, HEX);
}

void CVOutput::processMessage(const char* pMessage)
{
//	Serial.print("Message: ");
//	Serial.print(pMessage[0], HEX);
//	Serial.print(" No: ");
//	Serial.print(pMessage[1], DEC);
//	Serial.print(" Val: ");
//	Serial.println(pMessage[2], DEC);
	
	if ((*pMessage & 0xf0) == 0xb0) // Control Change
	{
		switch (pMessage[1])
		{
			case 0: // min
				setMinimum(pMessage[2] * 2);
				break;
			case 1: // max
				setMaximum(pMessage[2] * 2);
				break;
			defaut:
				break;
		}
	}
}
