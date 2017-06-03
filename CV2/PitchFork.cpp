#include "PitchFork.h"
#include "MIDI.h"

// the CC numbers implemented in the Pitch Fork Remote Control firmware
enum
{
	RC_FOOTSWITCH_CC = 0,
	RC_SHIFT_CC = 1,
	RC_LATCH_CC = 2,
	RC_OCTAVE_CC = 3,
	RC_BLEND_CC = 4,
	RC_BLEND_CONTROL_TYPE_CC = 5,
	RC_LATCH_ENABLE_CC = 6,
	RC_OCTAVE_ENABLE_CC = 7,
	RC_SHIFT_ENABLE_CC = 8,
	RC_FOOTSWITCH_ENABLE_CC = 9
};

PitchFork::PitchFork()
: OnOffTriggerable(0, PITCHFORK_TRIGGER_INSTANCE_CC),
  triggeredState(false),
  shift(0),
  latch(false),
  octave(BOTH),
  blend(255),
  footswitchEnabled(false),
  shiftEnabled(false),
  latchEnabled(true),
  octaveEnabled(false),
  blendControl(LOCAL),
  startupUs(0),
  startupTxDone(false)
{
	triggeredState = isTriggered();
}

PitchFork::~PitchFork()
{
}

void PitchFork::setup()
{
	MIDI::instance().setCCListener(this, 0, PITCHFORK_SHIFT_CC);
	MIDI::instance().setCCListener(this, 0, PITCHFORK_LATCH_CC);
	MIDI::instance().setCCListener(this, 0, PITCHFORK_OCTAVE_CC);
	MIDI::instance().setCCListener(this, 0, PITCHFORK_BLEND_CC);
	// feature enable/disable on channel 1
	MIDI::instance().setCCListener(this, 1, PITCHFORK_TRIGGER_INSTANCE_CC);
	MIDI::instance().setCCListener(this, 1, PITCHFORK_SHIFT_CC);
	MIDI::instance().setCCListener(this, 1, PITCHFORK_LATCH_CC);
	MIDI::instance().setCCListener(this, 1, PITCHFORK_OCTAVE_CC);
	MIDI::instance().setCCListener(this, 1, PITCHFORK_BLEND_CC);
	
	OnOffTriggerable::setup();
}

void PitchFork::loop(const unsigned long usNow)
{
	if ( !startupTxDone && usNow - startupUs >= STARTUP_TX_DELAY_US)
	{
		txAllValues();
		startupTxDone = true;
	}
	
	boolean newTriggeredState = isTriggered();
	
	if (footswitchEnabled)
	{
		if (newTriggeredState != triggeredState)
		{
			triggeredState = newTriggeredState;
			txCCMessage(MIDI_CHANNEL, RC_FOOTSWITCH_CC, triggeredState ? 1 : 0);
			Serial.print("FSW trig: ");
			Serial.println(triggeredState ? 1: 0, DEC);
		}
	}
}

void PitchFork::txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const
{
//	Serial.print("PF txCCMessage value: ");
//	Serial.println(value, HEX);

	Serial2.write(ch | MIDI_CONTROL_CHANGE);
	Serial2.write(cc);
	Serial2.write(value);
}

void PitchFork::txAllValues() const
{
	txCCMessage(MIDI_CHANNEL, RC_SHIFT_CC, shift);
	txCCMessage(MIDI_CHANNEL, RC_SHIFT_ENABLE_CC, shiftEnabled ? 1 : 0);
	txCCMessage(MIDI_CHANNEL, RC_LATCH_CC, latch ? 1 : 0);
	txCCMessage(MIDI_CHANNEL, RC_LATCH_ENABLE_CC, latchEnabled ? 1 : 0);
	txCCMessage(MIDI_CHANNEL, RC_OCTAVE_CC, (uint8_t)octave);
	txCCMessage(MIDI_CHANNEL, RC_OCTAVE_ENABLE_CC, octaveEnabled ? 1 : 0);
	txCCMessage(MIDI_CHANNEL, RC_BLEND_CC, blend);
	txCCMessage(MIDI_CHANNEL, RC_BLEND_CONTROL_TYPE_CC, (uint8_t)blendControl);
	// no FSW value support (yet?)
	txCCMessage(MIDI_CHANNEL, RC_FOOTSWITCH_ENABLE_CC, footswitchEnabled ? 1 : 0);
}

void PitchFork::processCCMessage(const uint8_t channel,
							  const uint8_t controllerNumber,
							  const uint8_t value)
{
//	Serial.println("PF MIDI msg");
	
	switch (controllerNumber)
	{
		case PITCHFORK_SHIFT_CC:
			if (channel == 0)
			{
				shift = value; // shift ID 0-11
				txCCMessage(MIDI_CHANNEL, RC_SHIFT_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				shiftEnabled = value > 0 ? true : false;
				txCCMessage(MIDI_CHANNEL, RC_SHIFT_ENABLE_CC, value);
			}
			break;
		case PITCHFORK_LATCH_CC:
			if (channel == 0)
			{
				latch = value ? true : false;
				txCCMessage(MIDI_CHANNEL, RC_LATCH_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				latchEnabled = value ? true : false;
				txCCMessage(MIDI_CHANNEL, RC_LATCH_ENABLE_CC, value);
			}
			break;
		case PITCHFORK_OCTAVE_CC:
			if (channel == 0)
			{
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
				txCCMessage(MIDI_CHANNEL, RC_OCTAVE_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				octaveEnabled = value ? true : false;
				txCCMessage(MIDI_CHANNEL, RC_OCTAVE_ENABLE_CC, value);
			}
			break;
		case PITCHFORK_BLEND_CC:
			if (channel == 0)
			{
				blend = value;
				txCCMessage(MIDI_CHANNEL, RC_BLEND_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				blendControl = (BlendControl)value;
				txCCMessage(MIDI_CHANNEL, RC_BLEND_CONTROL_TYPE_CC, value);
			}
			break;
		case PITCHFORK_TRIGGER_INSTANCE_CC:
			if (channel == 0)
			{
				OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				footswitchEnabled = value ? true : false;
				txCCMessage(MIDI_CHANNEL, RC_FOOTSWITCH_ENABLE_CC, value);
			}
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
	}
}

/**
 * Returns the current value of the given controller number.
 */
uint8_t PitchFork::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case PITCHFORK_SHIFT_CC:
			if (channel == 0)
			{
				return shift;
			}
			else if (channel == 1) // feature enable/disable
			{
				return shiftEnabled ? 1 : 0;
			}
			break;
		case PITCHFORK_LATCH_CC:
			if (channel == 0)
			{
				return latch ? 1 : 0;
			}
			else if (channel == 1) // feature enable/disable
			{
				return latchEnabled ? 1 : 0;
			}
			break;
		case PITCHFORK_OCTAVE_CC:
			if (channel == 0)
			{
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
			}
			else if (channel == 1) // feature enable/disable
			{
				return octaveEnabled ? 1 : 0;
			}
			break;
		case PITCHFORK_BLEND_CC:
			if (channel == 0)
			{
				return blend;
			}
			else if (channel == 1) // feature control type
			{
				return blendControl;
			}
			break;
		case PITCHFORK_TRIGGER_INSTANCE_CC:
			if (channel == 0)
			{
				return OnOffTriggerable::getControllerValue(channel, controllerNumber);
			}
			else if (channel == 1) // feature enable/disable
			{
				return footswitchEnabled ? 1 : 0;
			}
			break;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
	
	return 0;
}
