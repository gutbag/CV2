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
  onValue(1),
  onState(false)
{
	setDefaults(false, false);
}

AxolotiMIDIValue::~AxolotiMIDIValue()
{
}

void AxolotiMIDIValue::setup()
{
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, cv2MidiChannel, CV2_AXOLOTI_MIDI_OUTPUT_ON_VALUE_CC);

	onValue = 1;
	onState = true; // so that off works
	off();
}

void AxolotiMIDIValue::loop(const unsigned long usNow)
{
	boolean triggered = isTriggered();
	if (triggered)
		on();
	else
		off();
}

void AxolotiMIDIValue::on()
{
	if ( ! onState)
	{
		txCCMessage(axolotiMidiChannel, controllerNumber, onValue);
		onState = true;
	}
}

void AxolotiMIDIValue::off()
{
	if (onState)
	{
		txCCMessage(axolotiMidiChannel, controllerNumber, 0);
		onState = false;
	}
}

void AxolotiMIDIValue::txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const
{
	//	Serial.print("Axo txCCMessage value: ");
	//	Serial.println(value, HEX);
	
	Serial2.write(ch | MIDI_CONTROL_CHANGE);
	Serial2.write(cc);
	Serial2.write(value);
}

void AxolotiMIDIValue::processCCMessage(const uint8_t channel,
							const uint8_t controllerNumber,
							const uint8_t value)
{
	//	Serial.print("AxolotiMIDIValue Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case CV2_AXOLOTI_MIDI_OUTPUT_ON_VALUE_CC:
			onValue = value;
			off();
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
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
