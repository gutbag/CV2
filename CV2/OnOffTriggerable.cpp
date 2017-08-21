#include "OnOffTriggerable.h"
#include "MIDI.h"

OnOffTriggerable::OnOffTriggerable(const uint8_t aMidiChannel, const uint8_t aTriggerInstanceCC)
: trigger(NULL),
  midiChannel(aMidiChannel),
  triggerInstanceCC(aTriggerInstanceCC),
  triggerInstanceCCValue(DO_NOT_SAVE_VALUE),
  nullTriggerOnState(false),
  triggerDefaultOn(false),
  debug(false)
{
	
}

OnOffTriggerable::~OnOffTriggerable()
{
}

boolean OnOffTriggerable::isTriggered()
{
	if (trigger == NULL)
		return nullTriggerOnState;
	else
		return trigger->isOn();
}

boolean OnOffTriggerable::isOn()
{
	return isTriggered();
}

void OnOffTriggerable::setup()
{
//	Serial.print("OnOffTriggerable::setup, ch: ");
//	Serial.print(midiChannel, DEC);
//	Serial.print(" No: ");
//	Serial.println(triggerInstanceCC, DEC);
	
	MIDI::instance().setCCListener(this, midiChannel, triggerInstanceCC);
}

void OnOffTriggerable::processCCMessage(const uint8_t channel,
					  const uint8_t controllerNumber,
					  const uint8_t value)
{
//	Serial.print("OnOffTriggerable Message, ch: ");
//	Serial.print(channel, DEC);
//	Serial.print(" No: ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" Val: ");
//	Serial.print(value, DEC);
//	Serial.print(" triggerInstanceCC: ");
//	Serial.println(triggerInstanceCC, DEC);
	
	if (controllerNumber == triggerInstanceCC)
	{
		triggerInstanceCCValue = value;
		if (triggerInstanceCCValue != 0)
		{
			trigger = &TriggeredOnOff::instance(triggerInstanceCCValue - 1);
			trigger->setDefaultOn(triggerDefaultOn);
			
			if (debug)
			{
				Serial.print("OOT 0x");
				Serial.print((unsigned int)this, HEX);
				Serial.print(" def: ");
				Serial.print(triggerDefaultOn, DEC);
				Serial.print(" on: ");
				Serial.println(trigger->isOn(), DEC);
			}
		}
		else
		{
			trigger = NULL;
		}
	}
}

uint8_t OnOffTriggerable::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	if (controllerNumber == triggerInstanceCC)
		return triggerInstanceCCValue;
	
	return DO_NOT_SAVE_VALUE;
}

void OnOffTriggerable::setDefaults(const boolean aNullTriggerOnState, const boolean aTriggerDefaultOn)
{
	nullTriggerOnState = aNullTriggerOnState;
	triggerDefaultOn = aTriggerDefaultOn;
}
