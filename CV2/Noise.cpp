#include "Noise.h"
#include "MIDI.h"
#include "Footswitch.h"
#include "EnvelopeFollower.h"
#include "TriggeredOnOff.h"

static Noise* pInstances[2] = {NULL, NULL};

Noise& Noise::instance(const uint8_t index)
{
	return *pInstances[index];
}

Noise::Noise(const uint8_t index, const uint8_t aMidiChannel)
: value(0),
  lastValueUs(0),
  valueDelayUs(1 * 1000 * 1000), // 1s
  amplitude(1),
  rateCCValue(DO_NOT_SAVE_VALUE),
  amplitudeCCValue(DO_NOT_SAVE_VALUE),
  enable(NULL),
  midiChannel(aMidiChannel),
  triggerInstanceCCValue(DO_NOT_SAVE_VALUE)
{
	pInstances[index] = this;
}

Noise::~Noise()
{
}

uint16_t Noise::getMinimum()
{
	return 0;
}

uint16_t Noise::getMaximum()
{
	return 255;
}

uint16_t Noise::getValue()
{
	return value;
}

void Noise::setup()
{
	MIDI::instance().setCCListener(this, midiChannel, NOISE_RATE_CC);
	MIDI::instance().setCCListener(this, midiChannel, NOISE_SMOOTHING_CC);
	MIDI::instance().setCCListener(this, midiChannel, NOISE_TRIGGER_INSTANCE_CC);
	
//	enable = &TriggeredOnOff::instance(NOISE_TRIGGER_MIDI_CHANNEL);
//	enable->setDefaultOn(true);
}

void Noise::loop(const unsigned long usNow)
{
	boolean enabled = (enable == NULL) || (enable != NULL ? enable->isOn() : true);
	
	if (enabled)
	{
		if (usNow - lastValueUs >= valueDelayUs) // time for a new value
		{
			// set the new range for random from the current value +/- 2*amplitudeCCValue
			// constrain to actual min/max
			int16_t maxDiff = (int16_t)(amplitude << 1);
			int16_t randMin = (int16_t)value - maxDiff;
			if (randMin < getMinimum())
				randMin = getMinimum();
			int16_t randMax = (int16_t)value + maxDiff;
			if (randMax > getMaximum())
				randMax = getMaximum();
				
			int16_t newValue = random(randMin, randMax);

			if (0)
			{
				Serial.print("value: ");
				Serial.print(value, DEC);
				Serial.print(" min: ");
				Serial.print(randMin, DEC);
				Serial.print(" max: ");
				Serial.print(randMax, DEC);
				Serial.print(" val: ");
				Serial.print(newValue, DEC);
				Serial.print(" diff: ");
				Serial.println((int16_t)value - (int16_t)newValue, DEC);
			}
			
			value = newValue;
			
			lastValueUs = usNow;
		}
	}
}

void Noise::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
	//	Serial.print("Noise Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case NOISE_TRIGGER_INSTANCE_CC:
			triggerInstanceCCValue = value;
			if (triggerInstanceCCValue != 0)
			{
				enable = &TriggeredOnOff::instance(triggerInstanceCCValue - 1);
				enable->setDefaultOn(true); // default is on
			}
			else
			{
				enable = NULL;
			}
			break;
		case NOISE_RATE_CC:
			rateCCValue = value;
			valueDelayUs = (127-rateCCValue) * 10 * 1000; // 10ms steps
			break;
		case NOISE_SMOOTHING_CC:
			amplitudeCCValue = value;
			amplitude = amplitudeCCValue;
			break;
		default:
			break;
	}
}

uint8_t Noise::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case NOISE_TRIGGER_INSTANCE_CC:
			return triggerInstanceCCValue;
		case NOISE_RATE_CC:
			return rateCCValue;
		case NOISE_SMOOTHING_CC:
			return amplitudeCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
