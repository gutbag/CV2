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
  minimum(0),
  maximum(255),
  sideChainMinimum(0),
  sideChainMaximum(255),
  pProvider(NULL),
  sourceTypeId(0),
  lastProviderValue(~0),
  pSideChainProvider(NULL),
  sideChainSourceTypeId(0),
  dirty(false),
  sideChainMode(MIN),
  sideChainModeId(0)
{
}

CVOutput::~CVOutput()
{
}

void CVOutput::setup()
{
	dac.setOutput(output, value);
	
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_MIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_MAX_CC);
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_SOURCE_CC);
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_SIDE_CHAIN_SOURCE_CC);
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_SIDE_CHAIN_MIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_SIDE_CHAIN_MAX_CC);
	MIDI::instance().setCCListener(this, midiChannel, CV_OUTPUT_SIDE_CHAIN_MODE_CC);
	
	// temp !!!!!!!
//	minimum = 51;
//	maximum = 153;
//	pProvider = &LFO::instance();
//	pSideChainProvider = &Expression::instance(0);
}

void CVOutput::loop(const unsigned int usNow)
{
	uint8_t newValue = minimum;
	
	if (pProvider != NULL)
	{
		uint16_t providerValue = pProvider->getValue();
		newValue = map(providerValue, pProvider->getMinimum(), pProvider->getMaximum(),
					   minimum, maximum);
	}
	
	if (pSideChainProvider != NULL)
	{
		switch (sideChainMode)
		{
			case MIN:
				if (sideChainMinimum < minimum)
				{
					uint8_t sideValue = map(pSideChainProvider->getValue(),
											pSideChainProvider->getMinimum(),
											pSideChainProvider->getMaximum(),
											minimum - 1, sideChainMinimum);
					uint8_t maxForCalculation = pProvider != NULL ? maximum : minimum;
					newValue = map(newValue,
								   minimum,
								   maxForCalculation,
								   sideValue,
								   maxForCalculation);
				}
				break;
			case MAX:
			{
				// if fixed value, max is min
				uint8_t maxForCalculation = pProvider != NULL ? maximum : minimum;
				if (sideChainMaximum > maxForCalculation)
				{
					uint8_t sideValue = map(pSideChainProvider->getValue(),
											pSideChainProvider->getMinimum(),
											pSideChainProvider->getMaximum(),
											maxForCalculation + 1, sideChainMaximum);
					
					newValue = map(newValue,
								   minimum,
								   maxForCalculation,
								   minimum,
								   sideValue);
				}
				break;
			}
			case RANGE:
			{
				uint8_t maxForCalculation = pProvider != NULL ? maximum : minimum;
				uint16_t sideChainValue = pSideChainProvider->getValue();
				uint16_t sideChainMinimum = pSideChainProvider->getMinimum();
				uint16_t sideChainMaximum = pSideChainProvider->getMaximum();
				
				uint16_t newMax = map(sideChainValue,
									  sideChainMinimum,
									  sideChainMaximum,
									  maxForCalculation + 1, sideChainMaximum);
				uint8_t newMin = map(sideChainValue,
									 sideChainMinimum,
									 sideChainMaximum,
									 sideChainMinimum, minimum - 1);
				newValue = map(sideChainValue,
							   sideChainMinimum,
							   sideChainMaximum,
							   newMin,
							   newMax);
				break;
			}
		}
	}
	
	dac.setOutput(output, newValue);
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
			sourceTypeId = value;
			switch (sourceTypeId)
			{
				case CV_OUTPUT_SOURCE_FIXED_VALUE:
					pProvider = NULL;
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_LFO1_VALUE:
					pProvider = &LFO::instance(0);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_LFO2_VALUE:
					pProvider = &LFO::instance(1);
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
				case CV_OUTPUT_SOURCE_ENV_VALUE:
					pProvider = &EnvelopeFollower::instance();
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_RAMP1_VALUE:
					pProvider = &Ramp::instance(0);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_RAMP2_VALUE:
					pProvider = &Ramp::instance(1);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_NOISE1_VALUE:
					pProvider = &Noise::instance(0);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_NOISE2_VALUE:
					pProvider = &Noise::instance(1);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_BUS1_VALUE:
					pProvider = &Bus::instance(0);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_BUS2_VALUE:
					pProvider = &Bus::instance(1);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_BUS3_VALUE:
					pProvider = &Bus::instance(2);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_BUS4_VALUE:
					pProvider = &Bus::instance(3);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_AWG1_VALUE:
					pProvider = &AWG::instance(0);
					dirty = true;
					break;
				case CV_OUTPUT_SOURCE_AWG2_VALUE:
					pProvider = &AWG::instance(1);
					dirty = true;
					break;
				default:
					break;
			}
			break;
		case CV_OUTPUT_SIDE_CHAIN_SOURCE_CC:
			sideChainSourceTypeId = value;
			switch (sideChainSourceTypeId)
			{
			case CV_OUTPUT_SIDE_CHAIN_SOURCE_NONE_VALUE:
				pSideChainProvider = NULL;
				dirty = true;
				break;
			case CV_OUTPUT_SIDE_CHAIN_SOURCE_LFO1_VALUE:
				pSideChainProvider = &LFO::instance(0);
				dirty = true;
				break;
			case CV_OUTPUT_SIDE_CHAIN_SOURCE_LFO2_VALUE:
				pSideChainProvider = &LFO::instance(1);
				dirty = true;
				break;
			case CV_OUTPUT_SIDE_CHAIN_SOURCE_EXPR1_VALUE:
				pSideChainProvider = &Expression::instance(0);
				dirty = true;
				break;
			case CV_OUTPUT_SIDE_CHAIN_SOURCE_EXPR2_VALUE:
				pSideChainProvider = &Expression::instance(1);
				dirty = true;
				break;
			default:
				break;
			}
			break;
		case CV_OUTPUT_SIDE_CHAIN_MIN_CC:
			sideChainMinimum = value * 2;
			break;
		case CV_OUTPUT_SIDE_CHAIN_MAX_CC:
			sideChainMaximum = value * 2;
			break;
		case CV_OUTPUT_SIDE_CHAIN_MODE_CC:
			sideChainModeId = value;
			switch (sideChainModeId)
			{
				case CV_OUTPUT_SIDE_CHAIN_MODE_MIN_VALUE:
					sideChainMode = MIN;
					dirty = true;
					break;
				case CV_OUTPUT_SIDE_CHAIN_MODE_MAX_VALUE:
					sideChainMode = MAX;
					dirty = true;
					break;
				case CV_OUTPUT_SIDE_CHAIN_MODE_RANGE_VALUE:
					sideChainMode = RANGE;
					dirty = true;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

uint8_t CVOutput::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case CV_OUTPUT_MIN_CC:
			return minimum >> 1;
		case CV_OUTPUT_MAX_CC:
			return maximum >> 1;
		case CV_OUTPUT_SOURCE_CC:
			return sourceTypeId;
		case CV_OUTPUT_SIDE_CHAIN_SOURCE_CC:
			return sideChainSourceTypeId;
		case CV_OUTPUT_SIDE_CHAIN_MIN_CC:
			return sideChainMinimum >> 1;
			break;
		case CV_OUTPUT_SIDE_CHAIN_MAX_CC:
			return sideChainMaximum >> 1;
			break;
		case CV_OUTPUT_SIDE_CHAIN_MODE_CC:
			return sideChainModeId;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
