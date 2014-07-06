#include "Freeze.h"
#include "OnOffStateProvider.h"
#include "Footswitch.h"
#include "MIDI.h"
#include "MIDIMessages.h"

Freeze::Freeze(const uint8_t aPin, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, FREEZE_TRIGGER_INSTANCE_CC),
  pin(aPin), midiChannel(aMidiChannel), onState(false)
{
	setDefaults(false, false);
}

Freeze::~Freeze()
{
}

void Freeze::setup()
{
	OnOffTriggerable::setup();
	pinMode(pin, OUTPUT);
	onState = true; // so that off works
	off();
}

void Freeze::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	if (triggered)
		on();
	else
		off();
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
