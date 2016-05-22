#include "PitchFork.h"
#include "MIDI.h"

/*
 CC n values:
  footswitch off
  footswitch on
  shift r/c disabled
  shift r/c enabled
  shift 0
  shift 1
  shift 2
  shift 3
  shift 4
  shift 5
  shift 6
  shift 7
  shift 8
  shift 9
  shift 10
  latch r/c enable
  latch on
  latch off (momentary)
  oct r/c disable
  oct r/c enable
  oct up
  oct down
  oct both
  blend r/c disabled
  blend r/c enabled
 */

// CV2 values for CC 
/*
 enum
{
	FOOTSWITCH_OFF,
	FOOTSWITCH_ON,
	SHIFT_RC_DISABLE,
	SHIFT_RC_ENABLE,
	SHIFT_0,
	SHIFT_1,
	SHIFT_2,
	SHIFT_3,
	SHIFT_4,
	SHIFT_5,
	SHIFT_6,
	SHIFT_7,
	SHIFT_8,
	SHIFT_9,
	SHIFT_10,
	LATCH_RC_DISABLE,
	LATCH_RC_ENABLE,
	LATCH_ON,
	LATCH_OFF,
	OCTAVE_RC_DISABLE,
	OCTAVE_RC_ENABLE,
	OCTAVE_UP,
	OCTAVE_DOWN,
	OCTAVE_BOTH,
	BLEND_RC_DISABLE,
	BLEND_RC_ENABLE
};
 */

// the CC numbers implemented in the Pitch Fork Remote Control firmware
enum
{
	RC_FOOTSWITCH_CC = 0,
	RC_SHIFT_CC = 1,
	RC_LATCH_CC = 2,
	RC_OCTAVE_CC = 3,
	RC_BLEND_CC = 4
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
  blendControl(LOCAL)
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
	
	Serial2.begin(31250);
//	midiOut.setup();

	OnOffTriggerable::setup();
}

void PitchFork::loop(const unsigned long usNow)
{
	boolean newTriggeredState = isTriggered();
	
	if (footswitchEnabled)
	{
		if (newTriggeredState != triggeredState)
		{
			triggeredState = newTriggeredState;
			txCCMessage(0, RC_FOOTSWITCH_CC, triggeredState ? 1 : 0);
			Serial.print("FSW trig: ");
			Serial.println(triggeredState ? 1: 0, DEC);
		}
	}
	
//	midiOut.loop(usNow);
}

void PitchFork::txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const
{
//	Serial.print("PF txCCMessage value: ");
//	Serial.println(value, HEX);

	Serial2.write(ch | MIDI_CONTROL_CHANGE);
	Serial2.write(cc);
	Serial2.write(value);
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
				txCCMessage(0, RC_SHIFT_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				shiftEnabled = value > 0 ? true : false;
				txCCMessage(1, RC_SHIFT_CC, value);
			}
			break;
		case PITCHFORK_LATCH_CC:
			if (channel == 0)
			{
				latch = value ? true : false;
				txCCMessage(0, RC_LATCH_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				latchEnabled = value ? true : false;
				txCCMessage(1, RC_LATCH_CC, value);
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
				txCCMessage(0, RC_OCTAVE_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				octaveEnabled = value ? true : false;
				txCCMessage(1, RC_OCTAVE_CC, value);
			}
			break;
		case PITCHFORK_BLEND_CC:
			if (channel == 0)
			{
				blend = value;
				txCCMessage(0, RC_BLEND_CC, value);
			}
			else if (channel == 1) // feature enable/disable
			{
				blendControl = (BlendControl)value;
				txCCMessage(1, RC_BLEND_CC, value);
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
				txCCMessage(1, RC_FOOTSWITCH_CC, value);
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
