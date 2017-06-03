#include "CVOutput.h"
#include "DAC.h"
#include "ValueProvider.h"
#include "MIDI.h"
#include "MIDIMessages.h"
#include "LFO.h"
#include "Expression.h"
#include "EnvelopeFollower.h"
#include "Ramp.h"
#include "Noise.h"
#include "Bus.h"
#include "AWG.h"

CVOutput::CVOutput(DAC& aDac, const uint8_t anOutput, const uint8_t aMidiChannel)
: dac(aDac),
  output(anOutput),
  midiChannel(aMidiChannel),
  value(0),
  pProvider(NULL),
  sourceTypeId(0),
  lowValue(aMidiChannel, CV_OUTPUT_LOW_MIN_CC, CV_OUTPUT_LOW_MAX_CC, CV_OUTPUT_LOW_SOURCE_CC),
  highValue(aMidiChannel, CV_OUTPUT_HIGH_MIN_CC, CV_OUTPUT_HIGH_MAX_CC, CV_OUTPUT_HIGH_SOURCE_CC)
{
}

CVOutput::~CVOutput()
{
}

void CVOutput::setup()
{
	dac.setOutput(output, value);
	
	lowValue.setup();
	lowValue.setMinimum(0);
	lowValue.setMaximum(127);
	highValue.setup();
	highValue.setMinimum(0);
	highValue.setMaximum(127);
	
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_SOURCE_CC);
}

void CVOutput::loop(const unsigned int usNow)
{
	uint8_t newValue = lowValue.getValue() << 1;
	
	if (pProvider != NULL)
	{
		uint16_t providerValue = pProvider->getValue();
		newValue = map(providerValue, pProvider->getMinimum(), pProvider->getMaximum(),
					   lowValue.getValue() << 1, highValue.getValue() << 1);
	}
	
	dac.setOutput(output, newValue);
}

//void CVOutput::setMinimum(const uint8_t value)
//{
////	minimum = value;
////	Serial.print("setMinimum: ");
////	Serial.println(minimum, HEX);
//}
//
//void CVOutput::setMaximum(const uint8_t value)
//{
////	maximum = value;
////	Serial.print("setMaximum: ");
////	Serial.println(maximum, HEX);
//}

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
		case CV_OUTPUT_SOURCE_CC:
			sourceTypeId = value;
			switch (sourceTypeId)
			{
				case CV_OUTPUT_SOURCE_FIXED_VALUE:
					pProvider = NULL;
					break;
				case CV_OUTPUT_SOURCE_LFO1_VALUE:
					pProvider = &LFO::instance(0);
					break;
				case CV_OUTPUT_SOURCE_LFO2_VALUE:
					pProvider = &LFO::instance(1);
					break;
				case CV_OUTPUT_SOURCE_EXPR1_VALUE:
					pProvider = &Expression::instance(0);
					break;
				case CV_OUTPUT_SOURCE_EXPR2_VALUE:
					pProvider = &Expression::instance(1);
					break;
				case CV_OUTPUT_SOURCE_ENV_VALUE:
					pProvider = &EnvelopeFollower::instance();
					break;
				case CV_OUTPUT_SOURCE_RAMP1_VALUE:
					pProvider = &Ramp::instance(0);
					break;
				case CV_OUTPUT_SOURCE_RAMP2_VALUE:
					pProvider = &Ramp::instance(1);
					break;
				case CV_OUTPUT_SOURCE_NOISE1_VALUE:
					pProvider = &Noise::instance(0);
					break;
				case CV_OUTPUT_SOURCE_NOISE2_VALUE:
					pProvider = &Noise::instance(1);
					break;
				case CV_OUTPUT_SOURCE_BUS1_VALUE:
					pProvider = &Bus::instance(0);
					break;
				case CV_OUTPUT_SOURCE_BUS2_VALUE:
					pProvider = &Bus::instance(1);
					break;
				case CV_OUTPUT_SOURCE_BUS3_VALUE:
					pProvider = &Bus::instance(2);
					break;
				case CV_OUTPUT_SOURCE_BUS4_VALUE:
					pProvider = &Bus::instance(3);
					break;
				case CV_OUTPUT_SOURCE_AWG1_VALUE:
					pProvider = &AWG::instance(0);
					break;
				case CV_OUTPUT_SOURCE_AWG2_VALUE:
					pProvider = &AWG::instance(1);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

uint8_t CVOutput::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case CV_OUTPUT_SOURCE_CC:
			return sourceTypeId;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
