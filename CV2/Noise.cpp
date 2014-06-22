#include "Noise.h"
#include "MIDI.h"
#include "Footswitch.h"
#include "EnvelopeFollower.h"

static Noise* pInstance = NULL;

Noise& Noise::instance()
{
	return *pInstance;
}

Noise::Noise()
: value(0),
  lastValueUs(0),
  valueDelayUs(1 * 1000 * 1000), // 1s
  rateCCValue(0),
  amplitudeCCValue(1)
{
	pInstance = this;
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
	MIDI::instance().setCCListener(this, 0, NOISE_TRIGGER_MODE_CC);
	MIDI::instance().setCCListener(this, 0, NOISE_CONTROL_TYPE_CC);
	MIDI::instance().setCCListener(this, 0, NOISE_RATE_CC);
	MIDI::instance().setCCListener(this, 0, NOISE_SMOOTHING_CC);
}

void Noise::loop(const unsigned long usNow)
{
	boolean triggered = true;
	
	if (triggered)
	{
		if (usNow - lastValueUs >= valueDelayUs) // time for a new value
		{
			// set the new range for random from the current value +/- 2*amplitudeCCValue
			// constrain to actual min/max
			int16_t maxDiff = (int16_t)(amplitudeCCValue << 1);
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
		case NOISE_RATE_CC:
			rateCCValue = value;
			valueDelayUs = value * 10 * 1000; // 10ms steps
			break;
		case NOISE_SMOOTHING_CC:
			amplitudeCCValue = value;
			break;
		default:
			break;
	}
}

uint8_t Noise::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case NOISE_RATE_CC:
			return rateCCValue;
		case NOISE_SMOOTHING_CC:
			return amplitudeCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
