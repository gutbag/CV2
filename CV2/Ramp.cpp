#include "Ramp.h"
#include "MIDI.h"

static Ramp* pInstances[2] = {NULL, NULL};

Ramp& Ramp::instance(const uint8_t index)
{
	return *pInstances[index];
}

Ramp::Ramp(const uint8_t index, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, RAMP_TRIGGER_INSTANCE_CC),
  value(0),
  invert(false),
  state(IDLE),
  rampTimeCCValue(DO_NOT_SAVE_VALUE),
  lastValueUs(0),
  valueDelayUs(0),
  startUs(0),
  rampTimeUs(2 * 1000000),
  midiChannel(aMidiChannel)
{
	pInstances[index] = this;
	setDefaults(false, false);
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
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, midiChannel, RAMP_TIME_CC);
	MIDI::instance().setCCListener(this, midiChannel, RAMP_INVERT_CC);
	
	state = IDLE;
	calcValueDelay();
	setIdleValue();
}

void Ramp::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	
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
					if (invert)
					{
						if (value == getMinimum())
							state = FINISHED;
						else
							value--;
					}
					else
					{
						if (value == getMaximum())
							state = FINISHED;
						else
							value++;
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
	if (invert)
		value = getMaximum();
	else
		value = getMinimum();
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
		case RAMP_INVERT_CC:
			invert = value == 1 ? true : false;
			setIdleValue();
			break;
		case RAMP_TIME_CC:
			rampTimeCCValue= value;
			rampTimeUs = value * 100000; // value is in 100ms steps
			calcValueDelay();
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t Ramp::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case RAMP_INVERT_CC:
			return invert ? 1 : 0;
		case RAMP_TIME_CC:
			return rampTimeCCValue;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
}
