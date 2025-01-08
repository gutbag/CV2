#include "Footswitch.h"
#include "MIDI.h"

static Footswitch* pInstances[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

Footswitch& Footswitch::instance(const uint8_t index)
{
	return *pInstances[index];
}

Footswitch::Footswitch(const uint8_t instanceIndex, const uint8_t aMidiChannel, const uint8_t aSwPin, const uint8_t anLedPin)
: Switch(aSwPin), ledPin(anLedPin), midiChannel(aMidiChannel), onOffInstanceNumber(0), pOnOff(NULL), ledIsOn(false)
{
	pInstances[instanceIndex] = this;
}

Footswitch::~Footswitch()
{
	
}

void Footswitch::setup()
{
	Switch::setup();
	
	pinMode(ledPin, OUTPUT);
	setLed(false, true);

	MIDI::instance().setCCListener(this, midiChannel, FOOTSWITCH_TRIGGER_INSTANCE_CC);
}

void Footswitch::loop(const unsigned long usNow)
{
	Switch::loop(usNow);
	
	if (pOnOff != NULL)
	{
		setLed(pOnOff->isOn());
	}
}

void Footswitch::setLed(const boolean on, const boolean force /*=false*/)
{
	if (force || (ledIsOn != on))
	{
		digitalWrite(ledPin, on ? HIGH : LOW);
		ledIsOn = on;
	}
}

void Footswitch::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
	//	Serial.print("Footswitch Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case FOOTSWITCH_TRIGGER_INSTANCE_CC:
			onOffInstanceNumber = value;
			pOnOff = NULL;
			if (onOffInstanceNumber > 0)
				pOnOff = &TriggeredOnOff::instance(onOffInstanceNumber - 1);
			break;
		default:
			break;
	}
}

uint8_t Footswitch::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case FOOTSWITCH_TRIGGER_INSTANCE_CC:
			return onOffInstanceNumber;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}

void Footswitch::setOn(const boolean state)
{
	if (pOnOff != NULL)
		pOnOff->setOn(state);
}
