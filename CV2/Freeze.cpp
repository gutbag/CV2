#include "Freeze.h"
#include "OnOffStateProvider.h"
#include "Footswitch.h"
#include "MIDI.h"
#include "MIDIMessages.h"
//#include "EnvelopeFollower.h"
#include "TriggeredOnOff.h"

Freeze::Freeze(const uint8_t aPin, const uint8_t aMidiChannel)
: pin(aPin), midiChannel(aMidiChannel), onState(false), onOff(NULL)
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
	
	onOff = &TriggeredOnOff::instance(midiChannel);
	onOff->setDefaultOn(false); // default is off
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
