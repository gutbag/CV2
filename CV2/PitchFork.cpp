#include "PitchFork.h"
#include "MIDI.h"

// the CC numbers implemented in the Pitch Fork Remote Control firmware
enum
{
	RC_SHIFT_CC = 0x00,
	RC_LATCH_CC = 0x01,
	RC_OCTAVE_CC = 0x02,
	RC_BLEND_CC = 0x03
};

static const uint8_t RC_FOOTSWITCH_CC = 0; //  TODO get from remote control firmware

PitchFork::PitchFork(const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, PITCHFORK_TRIGGER_INSTANCE_CC),
  triggeredState(false),
  midiChannel(aMidiChannel),
  shift(0),
  latchEnabled(true),
  octave(BOTH),
  blend(255)
{
	triggeredState = isTriggered();
}

PitchFork::~PitchFork()
{	
}

void PitchFork::setup()
{
	MIDI::instance().setCCListener(this, midiChannel, PITCHFORK_SHIFT_CC);
	MIDI::instance().setCCListener(this, midiChannel, PITCHFORK_LATCH_CC);
	MIDI::instance().setCCListener(this, midiChannel, PITCHFORK_OCTAVE_CC);
	MIDI::instance().setCCListener(this, midiChannel, PITCHFORK_BLEND_CC);
	
	Serial2.begin(31250);
}

void PitchFork::loop(const unsigned long usNow)
{
	boolean newTriggeredState = isTriggered();
	
	if (newTriggeredState != triggeredState)
	{
		txCCMessage(RC_FOOTSWITCH_CC, triggeredState ? 1 : 0);
		triggeredState = newTriggeredState;
	}
}

void PitchFork::txCCMessage(const uint8_t cc, const uint8_t value) const
{
	Serial2.write(midiChannel | MIDI_CONTROL_CHANGE);
	Serial2.write(cc);
	Serial2.write(value);
}

void PitchFork::processCCMessage(const uint8_t channel,
							  const uint8_t controllerNumber,
							  const uint8_t value)
{
	switch (controllerNumber)
	{
		case PITCHFORK_SHIFT_CC:
			shift = value * 2;
			txCCMessage(RC_SHIFT_CC, value);
			break;
		case PITCHFORK_LATCH_CC:
			latchEnabled = value ? true : false;
			txCCMessage(RC_LATCH_CC, value);
			break;
		case PITCHFORK_OCTAVE_CC:
			switch (value)
			{
			case 0:
				octave = UP;
				break;
			case 1:
				octave = DOWN;
				break;
			case 2:
				octave = BOTH;
				break;
			default:
				octave = BOTH;
				break;
			}
			txCCMessage(RC_OCTAVE_CC, value);
			break;
		case PITCHFORK_BLEND_CC:
			blend = value * 2;
			txCCMessage(RC_BLEND_CC, value);
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

/**
 * Returns the current value of the given controller number. If the MSB is
 * set in the return value, it should not be saved (TODO: review whether
 * this should be controlled separately).
 */
uint8_t PitchFork::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case PITCHFORK_SHIFT_CC:
			return shift / 2;
		case PITCHFORK_LATCH_CC:
			return latchEnabled ? 1 : 0;
		case PITCHFORK_OCTAVE_CC:
			switch (octave)
			{
			case UP:
				return 0;
			case DOWN:
				return 1;
			case BOTH:
				return 2;
			default:
				return 2;
			}
			break;
		case PITCHFORK_BLEND_CC:
			return blend / 2;
		default:
			return OnOffTriggerable::getControllerValue(controllerNumber);
	}
}
