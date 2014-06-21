#include "Ramp.h"
#include "MIDI.h"
#include "Footswitch.h"
#include "EnvelopeFollower.h"

static Ramp* pInstance = NULL;

Ramp& Ramp::instance()
{
	return *pInstance;
}

Ramp::Ramp()
: value(0),
  startAtMin(true),
  state(IDLE),
  triggerModeCCValue(DO_NOT_SAVE_VALUE),
  controlTypeCCValue(DO_NOT_SAVE_VALUE),
  directionCCValue(DO_NOT_SAVE_VALUE),
  rampTimeCCValue(DO_NOT_SAVE_VALUE),
  lastValueUs(0),
  valueDelayUs(0),
  startUs(0),
  rampTimeUs(2 * 1000000)
{
	pInstance = this;
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
	trigger.setup();
	
	// TEMP
//	trigger.setMode(TriggeredOnOff::LEVEL_INV);
//	trigger.setProvider(&Footswitch::instance(0));
	
	// default to FSW 1
	trigger.setProvider(&Footswitch::instance(0));
	
	MIDI::instance().setCCListener(this, 0, RAMP_TRIGGER_MODE_CC);
	MIDI::instance().setCCListener(this, 0, RAMP_CONTROL_TYPE_CC);
	MIDI::instance().setCCListener(this, 0, RAMP_TIME_CC);
	MIDI::instance().setCCListener(this, 0, RAMP_DIRECTION_CC);
	
	state = IDLE;
	calcValueDelay();
	setIdleValue();
}

void Ramp::loop(const unsigned long usNow)
{
	trigger.loop(usNow);
	boolean triggered = trigger.isOn();
	
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
		case RAMP_TRIGGER_MODE_CC:
			triggerModeCCValue = value;
			switch (triggerModeCCValue)
			{
			case RAMP_TRIGGER_MODE_LEVEL_NON_INV_VALUE:
				trigger.setMode(TriggeredOnOff::LEVEL_NON_INV);
				break;
			case RAMP_TRIGGER_MODE_LEVEL_INV_VALUE:
				trigger.setMode(TriggeredOnOff::LEVEL_INV);
				break;
			case RAMP_TRIGGER_MODE_EDGE_OFF_ON_VALUE:
				trigger.setMode(TriggeredOnOff::EDGE_OFF_ON);
				break;
			case RAMP_TRIGGER_MODE_EDGE_ON_OFF_VALUE:
				trigger.setMode(TriggeredOnOff::EDGE_ON_OFF);
				break;
			default:
				break;
			}
			break;
		case RAMP_CONTROL_TYPE_CC:
			controlTypeCCValue = value;
			switch (controlTypeCCValue)
			{
			case RAMP_CONTROL_FSW_1_VALUE:
				trigger.setProvider(&Footswitch::instance(0));
				break;
			case RAMP_CONTROL_FSW_2_VALUE:
				trigger.setProvider(&Footswitch::instance(1));
				break;
			case RAMP_CONTROL_FSW_3_VALUE:
				trigger.setProvider(&Footswitch::instance(2));
				break;
			case RAMP_CONTROL_ENV_VALUE:
				trigger.setProvider(&EnvelopeFollower::instance().getEnvelopeState());
				break;
			default:
				break;
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
		case RAMP_TRIGGER_MODE_CC:
			return triggerModeCCValue;
		case RAMP_CONTROL_TYPE_CC:
			return controlTypeCCValue;
		case RAMP_DIRECTION_CC:
			return directionCCValue;
		case RAMP_TIME_CC:
			return rampTimeCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
