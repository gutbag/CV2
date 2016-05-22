#include "AWG.h"
#include "MIDI.h"

static AWG* pInstances[2] = {NULL, NULL};

AWG& AWG::instance(const uint8_t index)
{
	return *pInstances[index];
}

AWG::AWG(const uint8_t index, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, AWG_TRIGGER_INSTANCE_CC),
  midiChannel(aMidiChannel),
  value(0),
  lastValueUs(0),
  running(false),
  loopEnabled(false),
  msPerTimeStep(midiChannel, AWG_MS_PER_TIME_STEP_MIN_CC, AWG_MS_PER_TIME_STEP_MAX_CC, AWG_MS_PER_TIME_STEP_SOURCE_CC),
  invert(false),
  currentPhase(0)
{
	pInstances[index] = this;
	
	for (uint8_t i=0; i<NUM_PHASES; i++)
	{
		phases[i].endPoint = 0;
		phases[i].sustain = false;
		phases[i].incrementUs = 0;
		phases[i].startUs = 0;
		phases[i].durationUs = 0;
		phases[i].increment = 1;
		phases[i].duration = 0;
	}

	setDefaults(true, true);
}

AWG::~AWG()
{
}

uint16_t AWG::getMinimum()
{
	return 0;
}

uint16_t AWG::getMaximum()
{
	return 255;
}

uint16_t AWG::getValue()
{
	return invert ? (getMaximum() - value) : value;
}

void AWG::setup()
{
	OnOffTriggerable::setup();
	msPerTimeStep.setup();
	msPerTimeStep.setMinimum(0);
	msPerTimeStep.setMaximum(127);
	
	MIDI::instance().setCCListener(this, midiChannel, AWG_LOOP_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_INVERT_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH1_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH2_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH3_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH4_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH5_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH6_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH7_SUSTAIN_CC);
	//MIDI::instance().setCCListener(this, midiChannel, AWG_PH8_SUSTAIN_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH1_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH2_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH3_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH4_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH5_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH6_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH7_END_AMPLITUDE_CC);
	//MIDI::instance().setCCListener(this, midiChannel, AWG_PH8_END_AMPLITUDE_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH1_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH2_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH3_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH4_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH5_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH6_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH7_DURATION_CC);
	MIDI::instance().setCCListener(this, midiChannel, AWG_PH8_DURATION_CC);
}

// applies increment to value and returns true if the endpoint is reached
boolean AWG::incrementValue(uint16_t& value, Phase& p)
{
	boolean reachedEndpoint = false;
	
	if (p.increment > 0)
	{
		if (value >= p.endPoint) // already at end, nothing to do
			reachedEndpoint = true;
		else
		{
			value += p.increment;
			if (value >= p.endPoint) // that was the last increment
				reachedEndpoint = true;
		}
	}
	else
	{
		if (value <= p.endPoint) // already at end, nothing to do
			reachedEndpoint = true;
		else
		{
			value += p.increment; // increment value is signed
			if (value <= p.endPoint) // that was the last increment
				reachedEndpoint = true;
		}
	}
	
	return reachedEndpoint;
}

void AWG::loop(const unsigned long usNow)
{
	if (running)
	{
		Phase& p = phases[currentPhase];
		
		if (usNow - lastValueUs >= p.incrementUs) // time for a new value
		{
			boolean reachedEndpoint = incrementValue(value, p);
			
			if (reachedEndpoint)
			{
				boolean triggered = isTriggered();
				
				if ( !p.sustain || !triggered) // move to next phase
				{
					if (currentPhase == (NUM_PHASES - 1)) // this is the last phase
					{
						if (loopEnabled && triggered)
						{
							startPhase(0, usNow);
						}
						else
						{
							running = false;
						}
					}
					else // move to next phase
					{
						startPhase(currentPhase + 1, usNow);
					}
				}
			}
			
			lastValueUs = usNow;
		}
	}
	else // not running, check trigger
	{
		if (isTriggered()) // start
		{
			value = 0;
			startPhase(0, usNow);
			lastValueUs = usNow;
			running = true;
		}
	}
}

// sets up to start running the phase with the given index
// if not 0, assumes that the current 'value' can be used
void AWG::startPhase(const uint8_t phaseIndex, const unsigned long usNow)
{
	currentPhase = phaseIndex;
	Phase& p = phases[currentPhase];
	p.durationUs = p.duration * msPerTimeStep.getValue() * 1000;
	p.startUs = usNow;
	uint16_t startValue = currentPhase == 0 ? 0 : value; // start from where we are unless phase 0
	int16_t amplDiff = p.endPoint - startValue;
	if (amplDiff == 0)
	{
		p.incrementUs = p.durationUs;
	}
	else if (amplDiff < 0)
	{
		p.incrementUs = p.durationUs / (startValue - p.endPoint);
		p.increment = -1;
	}
	else
	{
		p.incrementUs = p.durationUs / (p.endPoint - startValue);
		p.increment = 1;
	}
	
	if (0)
	{
		Serial.print("[");
		Serial.print(currentPhase, DEC);
		Serial.print("] incrementUs ");
		Serial.print(p.incrementUs, DEC);
		Serial.print(" endPoint ");
		Serial.print(p.endPoint, DEC);
		Serial.print(" startUs ");
		Serial.print(p.startUs, DEC);
		Serial.print(" durationUs ");
		Serial.print(p.durationUs, DEC);
		Serial.print(" increment ");
		Serial.print(p.increment, DEC);
		Serial.print(" value ");
		Serial.print(value, DEC);
		Serial.print(" startValue ");
		Serial.print(startValue, DEC);
		Serial.print(" amplDiff ");
		Serial.println(amplDiff, DEC);
	}
	
	if (currentPhase == 0) // just in case
		value = getMinimum(); // TODO: consider invert
}

void AWG::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
//	Serial.print("AWG Message, ch: ");
//	Serial.print(channel, DEC);
//	Serial.print(" No: ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" Val: ");
//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case AWG_LOOP_CC:
			loopEnabled = value != 0;
			break;
		case AWG_INVERT_CC:
			invert = value != 0;
			break;
		case AWG_PH1_SUSTAIN_CC:
			phases[0].sustain = value != 0;
			break;
		case AWG_PH2_SUSTAIN_CC:
			phases[1].sustain = value != 0;
			break;
		case AWG_PH3_SUSTAIN_CC:
			phases[2].sustain = value != 0;
			break;
		case AWG_PH4_SUSTAIN_CC:
			phases[3].sustain = value != 0;
			break;
		case AWG_PH5_SUSTAIN_CC:
			phases[4].sustain = value != 0;
			break;
		case AWG_PH6_SUSTAIN_CC:
			phases[5].sustain = value != 0;
			break;
		case AWG_PH7_SUSTAIN_CC:
			phases[6].sustain = value != 0;
			break;
		case AWG_PH1_END_AMPLITUDE_CC:
			phases[0].endPoint = value << 1;
			break;
		case AWG_PH2_END_AMPLITUDE_CC:
			phases[1].endPoint = value << 1;
			break;
		case AWG_PH3_END_AMPLITUDE_CC:
			phases[2].endPoint = value << 1;
			break;
		case AWG_PH4_END_AMPLITUDE_CC:
			phases[3].endPoint = value << 1;
			break;
		case AWG_PH5_END_AMPLITUDE_CC:
			phases[4].endPoint = value << 1;
			break;
		case AWG_PH6_END_AMPLITUDE_CC:
			phases[5].endPoint = value << 1;
			break;
		case AWG_PH7_END_AMPLITUDE_CC:
			phases[6].endPoint = value << 1;
			break;
		case AWG_PH1_DURATION_CC:
			phases[0].duration = value;
			break;
		case AWG_PH2_DURATION_CC:
			phases[1].duration = value;
			break;
		case AWG_PH3_DURATION_CC:
			phases[2].duration = value;
			break;
		case AWG_PH4_DURATION_CC:
			phases[3].duration = value;
			break;
		case AWG_PH5_DURATION_CC:
			phases[4].duration = value;
			break;
		case AWG_PH6_DURATION_CC:
			phases[5].duration = value;
			break;
		case AWG_PH7_DURATION_CC:
			phases[6].duration = value;
			break;
		case AWG_PH8_DURATION_CC:
			phases[7].duration = value;
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t AWG::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case AWG_LOOP_CC:
			return loopEnabled ? 1 : 0;
		case AWG_INVERT_CC:
			return invert ? 1 : 0;
		case AWG_PH1_SUSTAIN_CC:
			return phases[0].sustain ? 1 : 0;
		case AWG_PH2_SUSTAIN_CC:
			return phases[1].sustain ? 1 : 0;
		case AWG_PH3_SUSTAIN_CC:
			return phases[2].sustain ? 1 : 0;
		case AWG_PH4_SUSTAIN_CC:
			return phases[3].sustain ? 1 : 0;
		case AWG_PH5_SUSTAIN_CC:
			return phases[4].sustain ? 1 : 0;
		case AWG_PH6_SUSTAIN_CC:
			return phases[5].sustain ? 1 : 0;
		case AWG_PH7_SUSTAIN_CC:
			return phases[6].sustain ? 1 : 0;
		case AWG_PH1_END_AMPLITUDE_CC:
			return phases[0].endPoint >> 1;
		case AWG_PH2_END_AMPLITUDE_CC:
			return phases[1].endPoint >> 1;
		case AWG_PH3_END_AMPLITUDE_CC:
			return phases[2].endPoint >> 1;
		case AWG_PH4_END_AMPLITUDE_CC:
			return phases[3].endPoint >> 1;
		case AWG_PH5_END_AMPLITUDE_CC:
			return phases[4].endPoint >> 1;
		case AWG_PH6_END_AMPLITUDE_CC:
			return phases[5].endPoint >> 1;
		case AWG_PH7_END_AMPLITUDE_CC:
			return phases[6].endPoint >> 1;
		case AWG_PH1_DURATION_CC:
			return phases[0].duration;
		case AWG_PH2_DURATION_CC:
			return phases[1].duration;
		case AWG_PH3_DURATION_CC:
			return phases[2].duration;
		case AWG_PH4_DURATION_CC:
			return phases[3].duration;
		case AWG_PH5_DURATION_CC:
			return phases[4].duration;
		case AWG_PH6_DURATION_CC:
			return phases[5].duration;
		case AWG_PH7_DURATION_CC:
			return phases[6].duration;
		case AWG_PH8_DURATION_CC:
			return phases[7].duration;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
}
