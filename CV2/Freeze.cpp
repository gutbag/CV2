#include "Freeze.h"
#include "OnOffStateProvider.h"
#include "Footswitch.h"
#include "MIDI.h"
#include "MIDIMessages.h"
#include "EnvelopeFollower.h"

Freeze::Freeze(const uint8_t aPin, const uint8_t aMidiChannel)
: pin(aPin), midiChannel(aMidiChannel), onState(false), triggerModeCCValue(DO_NOT_SAVE_VALUE), controlTypeCCValue(DO_NOT_SAVE_VALUE)
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
	
	MIDI::instance().setCCListener(this, midiChannel, FREEZE_TRIGGER_MODE_CC);
	MIDI::instance().setCCListener(this, midiChannel, FREEZE_CONTROL_TYPE_CC);
	
	// TEMP - map to a footswitch
//	onOff.setProvider(&Footswitch::instance(midiChannel));
//	if (midiChannel == 0)
//		onOff.setMode(TriggeredOnOff::EDGE_OFF_ON);
//	else
//		onOff.setMode(TriggeredOnOff::EDGE_ON_OFF);

	onOff.setup();
}

void Freeze::loop(const unsigned long usNow)
{
	onOff.loop(usNow);
	
	if (onOff.isOn())
		on();
	else
		off();
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
		case FREEZE_TRIGGER_MODE_CC:
			triggerModeCCValue = value;
			switch (triggerModeCCValue)
			{
			case FREEZE_TRIGGER_MODE_LEVEL_NON_INV_VALUE:
				onOff.setMode(TriggeredOnOff::LEVEL_NON_INV);
				break;
			case FREEZE_TRIGGER_MODE_LEVEL_INV_VALUE:
				onOff.setMode(TriggeredOnOff::LEVEL_INV);
				break;
			case FREEZE_TRIGGER_MODE_EDGE_OFF_ON_VALUE:
				onOff.setMode(TriggeredOnOff::EDGE_OFF_ON);
				break;
			case FREEZE_TRIGGER_MODE_EDGE_ON_OFF_VALUE:
				onOff.setMode(TriggeredOnOff::EDGE_ON_OFF);
				break;
			default:
				break;
			}
			break;
		case FREEZE_CONTROL_TYPE_CC:
			controlTypeCCValue = value;
			switch (controlTypeCCValue)
			{
			case FREEZE_CONTROL_NONE_VALUE:
				onOff.setProvider(NULL);
				break;
			case FREEZE_CONTROL_FSW_1_VALUE:
				onOff.setProvider(&Footswitch::instance(0));
				break;
			case FREEZE_CONTROL_FSW_2_VALUE:
				onOff.setProvider(&Footswitch::instance(1));
				break;
			case FREEZE_CONTROL_FSW_3_VALUE:
				onOff.setProvider(&Footswitch::instance(2));
				break;
			case FREEZE_CONTROL_ENV_VALUE:
				onOff.setProvider(&EnvelopeFollower::instance().getEnvelopeState());
				break;
			default:
				break;
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
		case FREEZE_TRIGGER_MODE_CC:
			return triggerModeCCValue;
		case FREEZE_CONTROL_TYPE_CC:
			return controlTypeCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
	}
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
