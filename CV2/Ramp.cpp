#include "Ramp.h"
#include "MIDI.h"
#include "Footswitch.h"
#include "EnvelopeFollower.h"
#include "TriggeredOnOff.h"

static Ramp* pInstances[2] = {NULL, NULL};

Ramp& Ramp::instance(const uint8_t index)
{
	return *pInstances[index];
}

Ramp::Ramp(const uint8_t index, const uint8_t aMidiChannel)
: value(0),
  startAtMin(true),
  state(IDLE),
  trigger(NULL),
  directionCCValue(DO_NOT_SAVE_VALUE),
  rampTimeCCValue(DO_NOT_SAVE_VALUE),
  lastValueUs(0),
  valueDelayUs(0),
  startUs(0),
  rampTimeUs(2 * 1000000),
  midiChannel(aMidiChannel),
  triggerInstanceCCValue(DO_NOT_SAVE_VALUE)
{
	pInstances[index] = this;
}

Ramp::~Ramp()
{
}

uint16_t Ramp::getMinimum()
{
	return 0;
}

uint16_t Ramp::getMaximum()
{
	return 255;
}

uint16_t Ramp::getValue()
{
	return value;
}

void Ramp::setup()
{
	MIDI::instance().setCCListener(this, midiChannel, RAMP_TIME_CC);
	MIDI::instance().setCCListener(this, midiChannel, RAMP_DIRECTION_CC);
	MIDI::instance().setCCListener(this, midiChannel, RAMP_TRIGGER_INSTANCE_CC);
	
	//trigger = &TriggeredOnOff::instance(RAMP_TRIGGER_INSTANCE_CC);
	
	state = IDLE;
	calcValueDelay();
	setIdleValue();
}

void Ramp::loop(const unsigned long usNow)
{
	boolean triggered = trigger != NULL ? trigger->isOn() : false;
	
	switch (state)
	{
		case IDLE:
			if (triggered)
			{
				startUs = usNow;
				state = RUNNING;
			}
			break;
		case RUNNING:
			if (triggered)
			{
				if (usNow - lastValueUs >= valueDelayUs) // time for a new value
				{
					if (startAtMin)
					{
						if (value == getMaximum())
							state = FINISHED;
						else
							value++;
					}
					else
					{
						if (value == getMinimum())
							state = FINISHED;
						else
							value--;
					}
					
					lastValueUs = usNow;
				}
			}
			else // stop
			{
				state = FINISHED;
			}
			break;
		case FINISHED:
			if ( ! triggered) // back to IDLE
			{
				setIdleValue();
				state = IDLE;
			}
			break;
	}
}

void Ramp::setIdleValue()
{
	if (startAtMin)
		value = getMinimum();
	else
		value = getMaximum();
}

void Ramp::calcValueDelay()
{
	valueDelayUs = rampTimeUs / (getMaximum() - getMinimum());
}

void Ramp::processCCMessage(const uint8_t channel,
						   const uint8_t controllerNumber,
						   const uint8_t value)
{
//	Serial.print("Ramp Message, ch: ");
//	Serial.print(channel, DEC);
//	Serial.print(" No: ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" Val: ");
//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case RAMP_TRIGGER_INSTANCE_CC:
			triggerInstanceCCValue = value;
			if (triggerInstanceCCValue != 0)
			{
				trigger = &TriggeredOnOff::instance(triggerInstanceCCValue - 1);
				trigger->setDefaultOn(false); // default is off
			}
			else
			{
				trigger = NULL;
			}
			break;
		case RAMP_DIRECTION_CC:
			directionCCValue = value;
			switch (directionCCValue)
			{
			case RAMP_DIRECTION_UP_VALUE:
				startAtMin = true;
				setIdleValue();
				break;
			case RAMP_DIRECTION_DOWN_VALUE:
				startAtMin = false;
				setIdleValue();
				break;
			default:
				break;
			}
			break;
		case RAMP_TIME_CC:
			rampTimeCCValue= value;
			rampTimeUs = value * 100000; // value is in 100ms steps
			calcValueDelay();
			break;
		default:
			break;
	}
}

uint8_t Ramp::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case RAMP_TRIGGER_INSTANCE_CC:
			return triggerInstanceCCValue;
		case RAMP_DIRECTION_CC:
			return directionCCValue;
		case RAMP_TIME_CC:
			return rampTimeCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
