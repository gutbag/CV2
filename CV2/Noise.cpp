#include "Noise.h"
#include "MIDI.h"
#include "Footswitch.h"
#include "EnvelopeFollower.h"

static Noise* pInstances[2] = {NULL, NULL};

Noise& Noise::instance(const uint8_t index)
{
	return *pInstances[index];
}

Noise::Noise(const uint8_t index, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, NOISE_TRIGGER_INSTANCE_CC),
  value(0),
  lastValueUs(0),
  valueDelayUs(1 * 1000 * 1000), // 1s
  amplitude(1),
  rateCCValue(aMidiChannel, NOISE_RATE_MIN_CC, NOISE_RATE_MAX_CC, NOISE_RATE_SOURCE_CC),
  amplitudeCCValue(aMidiChannel, NOISE_SMOOTHING_MIN_CC, NOISE_SMOOTHING_MAX_CC, NOISE_SMOOTHING_SOURCE_CC),
  midiChannel(aMidiChannel)
{
	pInstances[index] = this;
	setDefaults(true, true);
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
	OnOffTriggerable::setup();
	rateCCValue.setup();
	rateCCValue.setMinimum(0);
	rateCCValue.setMaximum(127);
	amplitudeCCValue.setup();
	amplitudeCCValue.setMinimum(0);
	amplitudeCCValue.setMaximum(127);
//	MIDI::instance().setCCListener(this, midiChannel, NOISE_RATE_CC);
//	MIDI::instance().setCCListener(this, midiChannel, NOISE_SMOOTHING_CC);
}

void Noise::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	
	if (triggered)
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
			
			valueDelayUs = (127-rateCCValue.getValue()) * 10 * 1000; // 10ms steps
			amplitude = amplitudeCCValue.getValue();
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
//		case NOISE_RATE_CC:
//			rateCCValue = value;
//			valueDelayUs = (127-rateCCValue) * 10 * 1000; // 10ms steps
//			break;
//		case NOISE_SMOOTHING_CC:
//			amplitudeCCValue = value;
//			amplitude = amplitudeCCValue;
//			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t Noise::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
//		case NOISE_RATE_CC:
//			return rateCCValue;
//		case NOISE_SMOOTHING_CC:
//			return amplitudeCCValue;
		default:
			return OnOffTriggerable::getControllerValue(controllerNumber);
	}
}
