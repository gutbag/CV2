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
	
	// TODO: can't use output alone as channel - will clash with 9V CVs
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_MIN_CC);
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_MAX_CC);
	MIDI::instance().setCCListener(this, output, CV_OUTPUT_SOURCE_CC);
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

void CVOutput::processCCMessage(const uint8_t channel,
							   const uint8_t controllerNumber,
							   const uint8_t value)
{
	//	Serial.print("CVOutput Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(pvalue, DEC);
	
	switch (controllerNumber)
	{
		case CV_OUTPUT_MIN_CC:
			setMinimum(value * 2);
			break;
		case CV_OUTPUT_MAX_CC:
			setMaximum(value * 2);
			break;
		case CV_OUTPUT_SOURCE_CC:
		{
			switch (value)
			{
				case CV_OUTPUT_SOURCE_FIXED_VALUE:
					pProvider = NULL;
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_LFO_VALUE:
					pProvider = &LFO::instance();
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_EXPR1_VALUE:
					pProvider = &Expression::instance(0);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_EXPR2_VALUE:
					pProvider = &Expression::instance(1);
					dirty = true;
					break;
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}
