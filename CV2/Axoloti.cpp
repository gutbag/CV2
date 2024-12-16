#include "Axoloti.h"
#include "MIDI.h"
#include "MIDIMessages.h"
#include "ValueProvider.h"
#include "LFO.h"
#include "Expression.h"
#include "EnvelopeFollower.h"
#include "Ramp.h"
#include "Noise.h"
#include "Bus.h"
#include "AWG.h"

// the CC numbers supported by the Axoloti
enum
{
	AXOLOTI_PRESET_NO_CC = 0,
	AXOLOTI_VAR_CC = 1,
	AXOLOTI_CTRL_1_CC = 2,
	AXOLOTI_CTRL_2_CC = 3,
	AXOLOTI_CTRL_3_CC = 4,
	AXOLOTI_CTRL_4_CC = 5,
	AXOLOTI_CTRL_5_CC = 6,
	AXOLOTI_CTRL_6_CC = 7
};

Axoloti::Axoloti()
: startupUs(0),
  startupTxDone(false),
  pProvider(NULL),
  sourceTypeId(0),
  lowValue(0, CV2_AXOLOTI_VAR_LOW_MIN_CC, CV2_AXOLOTI_VAR_LOW_MAX_CC, CV2_AXOLOTI_VAR_LOW_SOURCE_CC),
  highValue(0, CV2_AXOLOTI_VAR_HIGH_MIN_CC, CV2_AXOLOTI_VAR_HIGH_MAX_CC, CV2_AXOLOTI_VAR_HIGH_SOURCE_CC),
  patchNumber(0),
  presetNumber(0),
  varValue(0),
  lastUpdateUs(0),
  axolotiMIDIValue1(0, AXOLOTI_MIDI_CHANNEL, AXOLOTI_CTRL_1_CC),
  axolotiMIDIValue2(1, AXOLOTI_MIDI_CHANNEL, AXOLOTI_CTRL_2_CC),
  axolotiMIDIValue3(2, AXOLOTI_MIDI_CHANNEL, AXOLOTI_CTRL_3_CC),
  axolotiMIDIValue4(3, AXOLOTI_MIDI_CHANNEL, AXOLOTI_CTRL_4_CC),
  axolotiMIDIValue5(4, AXOLOTI_MIDI_CHANNEL, AXOLOTI_CTRL_5_CC),
  axolotiMIDIValue6(5, AXOLOTI_MIDI_CHANNEL, AXOLOTI_CTRL_6_CC),
  debug(false)
{
}

Axoloti::~Axoloti()
{
}

void Axoloti::setup()
{
	MIDI::instance().setCCListener(this, 0, CV2_AXOLOTI_PATCH_NO_CC);
	MIDI::instance().setCCListener(this, 0, CV2_AXOLOTI_PRESET_NO_CC);
	MIDI::instance().setCCListener(this, 0, CV2_AXOLOTI_VAR_SOURCE_CC);
	
	lowValue.setup();
	lowValue.setMinimum(0);
	lowValue.setMaximum(127);
	highValue.setup();
	highValue.setMinimum(0);
	highValue.setMaximum(127);
	
	axolotiMIDIValue1.setup();
	axolotiMIDIValue2.setup();
	axolotiMIDIValue3.setup();
	axolotiMIDIValue4.setup();
	axolotiMIDIValue5.setup();
	axolotiMIDIValue6.setup();
}

void Axoloti::loop(const unsigned long usNow)
{
	if ( !startupTxDone && usNow - startupUs >= STARTUP_TX_DELAY_US)
	{
		txAllValues();
		startupTxDone = true;
	}
	
	if (startupTxDone && usNow - lastUpdateUs >= MIN_UPDATE_PERIOD_US)
	{
		uint8_t newValue = lowValue.getValue(); // for fixed value

		if (pProvider != NULL) // override newValue
		{
			uint16_t providerValue = pProvider->getValue();
			newValue = map(providerValue, pProvider->getMinimum(), pProvider->getMaximum(),
						   lowValue.getValue(), highValue.getValue());
		}
		
		if (newValue != varValue)
		{
			varValue = newValue;
			txCCMessage(AXOLOTI_MIDI_CHANNEL, AXOLOTI_VAR_CC, varValue);
		}
		
		lastUpdateUs = usNow;
	}
	
	axolotiMIDIValue1.loop(usNow);
	axolotiMIDIValue2.loop(usNow);
	axolotiMIDIValue3.loop(usNow);
	axolotiMIDIValue4.loop(usNow);
	axolotiMIDIValue5.loop(usNow);
	axolotiMIDIValue6.loop(usNow);
}

void Axoloti::txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const
{
	//	Serial.print("Axo txCCMessage value: ");
	//	Serial.println(value, HEX);
	
	Serial2.write(ch | MIDI_CONTROL_CHANGE);
	Serial2.write(cc);
	Serial2.write(value);
}

void Axoloti::txPCMessage(const uint8_t ch, const uint8_t programNumber) const
{
	//	Serial.print("Axo txPCMessage programNumber: ");
	//	Serial.println(programNumber, HEX);
	
	Serial2.write(ch | MIDI_PROGRAM_CHANGE);
	Serial2.write(programNumber);
	
	// have to allow the Axoloti time to respond to the PC to stop the following
	// CCs being lost
	delay(100);
}

void Axoloti::txAllValues() const
{
	txPCMessage(AXOLOTI_MIDI_CHANNEL, patchNumber);
	txCCMessage(AXOLOTI_MIDI_CHANNEL, AXOLOTI_PRESET_NO_CC, presetNumber);
	txCCMessage(AXOLOTI_MIDI_CHANNEL, AXOLOTI_VAR_CC, varValue);
}

void Axoloti::processCCMessage(const uint8_t channel,
							  const uint8_t controllerNumber,
							  const uint8_t value)
{
//	Serial.print("Axo MIDI msg ch ");
//	Serial.print(channel, DEC);
//	Serial.print(" CC ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" v ");
//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case CV2_AXOLOTI_PATCH_NO_CC:
			patchNumber = value;
			txPCMessage(AXOLOTI_MIDI_CHANNEL, patchNumber);
			break;
		case CV2_AXOLOTI_PRESET_NO_CC:
			presetNumber = value;
			txCCMessage(AXOLOTI_MIDI_CHANNEL, AXOLOTI_PRESET_NO_CC, presetNumber);
			break;
		case CV2_AXOLOTI_VAR_SOURCE_CC:
			sourceTypeId = value;
			varValue = ~0; // loop() will tx new value
			switch (sourceTypeId)
			{
			case CV_OUTPUT_SOURCE_FIXED_VALUE: // TODO: change CV_ prefix
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

/**
 * Returns the current value of the given controller number.
 */
uint8_t Axoloti::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case CV2_AXOLOTI_PATCH_NO_CC:
			return patchNumber;
		case CV2_AXOLOTI_PRESET_NO_CC:
			return presetNumber;
		case CV2_AXOLOTI_VAR_SOURCE_CC:
			return sourceTypeId;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
