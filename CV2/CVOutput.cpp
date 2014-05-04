#include "CVOutput.h"
#include "DAC.h"
#include "ValueProvider.h"
#include "MIDI.h"
#include "MIDIMessages.h"
#include "LFO.h"
#include "Expression.h"

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
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_MIN_CC);
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_MAX_CC);
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_SOURCE_FIXED_CC);
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_SOURCE_LFO_CC);
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_SOURCE_EXPR_CC);
}

void CVOutput::setValueProvider(ValueProvider* aValueProvider)
{
	pProvider = aValueProvider;
}

void CVOutput::loop(const unsigned int usNow)
{
	if (pProvider != NULL)
	{
		uint16_t providerValue = pProvider->getValue();
		
		if (dirty || providerValue != lastProviderValue)
		{
			// get value and scale
			// TODO: check map impl - do custom version?
			// TODO: handle min > max
			// TODO: store provider min/max when changed
			long value = map(providerValue, pProvider->getMinimum(), pProvider->getMaximum(),
							 minimum, maximum);
			
			dac.setOutput(output, value);
			
			lastProviderValue = providerValue;
			dirty = false;
			
//			Serial.print("CV1: ");
//			Serial.println(value, DEC);
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
	Serial.print("Message: ");
	Serial.print(pMessage[0], HEX);
	Serial.print(" No: ");
	Serial.print(pMessage[1], DEC);
	Serial.print(" Val: ");
	Serial.println(pMessage[2], DEC);
	
	if ((*pMessage & 0xf0) == 0xb0) // Control Change
	{
		switch (pMessage[1])
		{
			case CV_OUTPUT_MIN_CC:
				setMinimum(pMessage[2] * 2);
				break;
			case CV_OUTPUT_MAX_CC:
				setMaximum(pMessage[2] * 2);
				break;
			case CV_OUTPUT_SOURCE_FIXED_CC:
				pProvider = NULL;
				dirty = true;
				break;
			case CV_OUTPUT_SOURCE_LFO_CC:
				pProvider = &LFO::instance();
				dirty = true;
				break;
			case CV_OUTPUT_SOURCE_EXPR_CC:
			{
				uint8_t exprIndex = pMessage[2] == 1 ? 0 : 1;
				pProvider = &Expression::instance(exprIndex);
				dirty = true;
				break;
			}
			default:
				break;
		}
	}
}
