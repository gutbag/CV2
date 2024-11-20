#include "OnOffStateProvider.h"
#include "Footswitch.h"
#include "MIDI.h"
#include "MIDIMessages.h"
#include "AxolotiMIDIValue.hpp"

AxolotiMIDIValue::AxolotiMIDIValue(const uint8_t aCV2MidiChannel, const uint8_t anAxolotiMidiChannel, const uint8_t aControllerNumber)
: OnOffTriggerable(aCV2MidiChannel, CV2_AXOLOTI_MIDI_OUTPUT_TRIGGER_INSTANCE_CC),
  cv2MidiChannel(aCV2MidiChannel),
  axolotiMidiChannel(anAxolotiMidiChannel),
  controllerNumber(aControllerNumber),
  onValue(0), // same as off, by default
  onState(false),
  triggered(false),
  debug(false)
{
	setDefaults(true, false);
}

AxolotiMIDIValue::~AxolotiMIDIValue()
{
}

void AxolotiMIDIValue::setup()
{
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, cv2MidiChannel, CV2_AXOLOTI_MIDI_OUTPUT_ON_VALUE_CC);

	onValue = 0;
	off(true);
	
//	Serial.print("AMV ch ");
//	Serial.println(cv2MidiChannel, DEC);
}

void AxolotiMIDIValue::loop(const unsigned long usNow)
{
	//boolean triggered = isTriggered();
	boolean newTriggered = isTriggered();

	if (debug && triggered != newTriggered)
	{
		Serial.print("AMV ch ");
		Serial.print(cv2MidiChannel, DEC);
		Serial.print(" trig: ");
		Serial.println(newTriggered, DEC);
	}
	
	triggered = newTriggered;

	if (triggered)
		on();
	else
		off();
}

void AxolotiMIDIValue::on(const boolean force /*= false*/)
{
	if ( ! onState || force)
	{
		if (debug)
		{
			Serial.print("AMV ch ");
			Serial.print(cv2MidiChannel, DEC);
			Serial.println(" on");
		}
		txCCMessage(axolotiMidiChannel, controllerNumber, onValue);
		onState = true;
	}
}

void AxolotiMIDIValue::off(const boolean force /*= false*/)
{
	if (onState || force)
	{
		if (debug)
		{
			Serial.print("AMV ch ");
			Serial.print(cv2MidiChannel, DEC);
			Serial.println(" off");
		}
		txCCMessage(axolotiMidiChannel, controllerNumber, 0);
		onState = false;
	}
}

void AxolotiMIDIValue::txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const
{
	if (debug)
	{
		Serial.print("AMV Axo ch ");
		Serial.print(ch, DEC);
		Serial.print(" cc ");
		Serial.print(cc, DEC);
		Serial.print(" tx ");
		Serial.println(value, DEC);
	}
	
	Serial2.write(ch | MIDI_CONTROL_CHANGE);
	Serial2.write(cc);
	Serial2.write(value);
}

void AxolotiMIDIValue::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
	if (debug)
	{
		Serial.print("AxolotiMIDIValue Message, ch: ");
		Serial.print(channel, DEC);
		Serial.print(" No: ");
		Serial.print(controllerNumber, DEC);
		Serial.print(" Val: ");
		Serial.println(value, DEC);
	}
	
	switch (controllerNumber)
	{
		case CV2_AXOLOTI_MIDI_OUTPUT_ON_VALUE_CC:
			onValue = value;
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}

	off(true);
}

uint8_t AxolotiMIDIValue::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case CV2_AXOLOTI_MIDI_OUTPUT_ON_VALUE_CC:
			return onValue;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
}
