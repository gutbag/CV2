#include "Freeze.h"
#include "OnOffStateProvider.h"
#include "Footswitch.h"
#include "MIDI.h"
#include "MIDIMessages.h"
//#include "EnvelopeFollower.h"
#include "TriggeredOnOff.h"

Freeze::Freeze(const uint8_t aPin, const uint8_t aMidiChannel)
: pin(aPin), midiChannel(aMidiChannel), onState(false), onOff(NULL), triggerInstanceCCValue(0)
{
}

Freeze::~Freeze()
{
}

void Freeze::setup()
{
	pinMode(pin, OUTPUT);
	onState = true; // so that off works
	off();

	MIDI::instance().setCCListener(this, midiChannel, FREEZE_TRIGGER_INSTANCE_CC);
}

void Freeze::loop(const unsigned long usNow)
{
	if (onOff != NULL && onOff->isOn())
		on();
	else
		off();
	
//	if (midiChannel == FREEZE_1_TRIGGER_MIDI_CHANNEL)
//	{
//		Serial.print((unsigned long)onOff, HEX);
//		Serial.println(onState, DEC);
//	}
}

void Freeze::on()
{
	if ( ! onState)
	{
		digitalWrite(pin, HIGH);
		onState = true;
	}
}

void Freeze::off()
{
	if (onState)
	{
		digitalWrite(pin, LOW);
		onState = false;
	}
}

void Freeze::processCCMessage(const uint8_t channel,
						   const uint8_t controllerNumber,
						   const uint8_t value)
{
	//	Serial.print("Freeze Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(pvalue, DEC);
	
	switch (controllerNumber)
	{
		case FREEZE_TRIGGER_INSTANCE_CC:
			triggerInstanceCCValue = value;
			if (triggerInstanceCCValue != 0)
			{
				onOff = &TriggeredOnOff::instance(triggerInstanceCCValue - 1);
				onOff->setDefaultOn(false); // default is off
			}
			else
			{
				onOff = NULL;
			}
			break;
		default:
			break;
	}
}

uint8_t Freeze::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case FREEZE_TRIGGER_INSTANCE_CC:
			return triggerInstanceCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
			break;
	}
}
