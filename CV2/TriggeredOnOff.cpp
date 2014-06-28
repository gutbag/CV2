#include "TriggeredOnOff.h"
#include "MIDI.h"
#include "Footswitch.h"
#include "EnvelopeFollower.h"

/**
 * OnOffStateProvider with state changes triggered by either level or edge
 * changes in its own provider. Can be configured to follow the provider's level
 * or toggle when the provider's state changes. Either can be inverted.
 *
 * TODO: both edges is pointless - same as level, but consider adding 
 * configurable inter-on gap (then both would re-trigger Freeze)
 */

static TriggeredOnOff* pInstances[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

TriggeredOnOff& TriggeredOnOff::instance(const uint8_t index)
{
	return *pInstances[index];
}

TriggeredOnOff::TriggeredOnOff(const uint8_t aMidiChannel)
: pProvider(NULL), edgeProvider(NULL), mode(LEVEL_NON_INV), on(false),
  midiChannel(aMidiChannel),
  triggerModeCCValue(0), controlTypeCCValue(0),
  defaultOn(false),
  debugFlag(false)
{
	pInstances[midiChannel] = this;
}

TriggeredOnOff::~TriggeredOnOff()
{
}

void TriggeredOnOff::setup()
{
	MIDI::instance().setCCListener(this, midiChannel, TRIGGER_MODE_CC);
	MIDI::instance().setCCListener(this, midiChannel, TRIGGER_CONTROL_TYPE_CC);

	edgeProvider.setup();
}

void TriggeredOnOff::loop(const unsigned long usNow)
{
	if (pProvider == NULL)
		return;
	
	if (mode == LEVEL_NON_INV)
	{
		on = pProvider->isOn();
	}
	else if (mode == LEVEL_INV)
	{
		on =  ! pProvider->isOn();
	}
	else // edge
	{
		edgeProvider.loop(usNow);
		
		OnOffEdgeProvider::EdgeType edge = edgeProvider.getEdge();
		
		if (edge != OnOffEdgeProvider::NONE) // edge detected
		{
			if (mode == EDGE_OFF_ON && edge == OnOffEdgeProvider::OFF_ON ||
				mode == EDGE_ON_OFF && edge == OnOffEdgeProvider::ON_OFF)
			{
				on = !on;
			}
		}
	}
}

void TriggeredOnOff::setProvider(OnOffStateProvider* pOnOffStateProvider)
{
	pProvider = pOnOffStateProvider;
	edgeProvider.setOnOffProvider(pProvider);
	on = false;
}

void TriggeredOnOff::setOn(const boolean state)
{
	on = state;
}

void TriggeredOnOff::setDefaultOn(const boolean state)
{
	defaultOn = state;
}


boolean TriggeredOnOff::isOn()
{
	return on;
}

void TriggeredOnOff::setMode(const TriggeredOnOff::Mode aMode)
{
	mode = aMode;
}

void TriggeredOnOff::processCCMessage(const uint8_t channel,
					  const uint8_t controllerNumber,
					  const uint8_t value)
{
//	Serial.print("TriggeredOnOff Message, ch: ");
//	Serial.print(channel, DEC);
//	Serial.print(" No: ");
//	Serial.print(controllerNumber, DEC);
//	Serial.print(" Val: ");
//	Serial.print(value, DEC);
//	Serial.print(" defaultOn: ");
//	Serial.println(defaultOn, DEC);
	
	switch (controllerNumber)
	{
		case TRIGGER_CONTROL_TYPE_CC:
			controlTypeCCValue = value;
			switch (controlTypeCCValue)
			{
			case TRIGGER_CONTROL_NONE_VALUE:
				setProvider(NULL);
				setOn(defaultOn);
				break;
			case TRIGGER_CONTROL_FSW_1_VALUE:
				setProvider(&Footswitch::instance(0));
					debugFlag = true;
				break;
			case TRIGGER_CONTROL_FSW_2_VALUE:
				setProvider(&Footswitch::instance(1));
				break;
			case TRIGGER_CONTROL_FSW_3_VALUE:
				setProvider(&Footswitch::instance(2));
				break;
			case TRIGGER_CONTROL_ENV_VALUE:
				setProvider(&EnvelopeFollower::instance().getEnvelopeState());
				break;
			default:
				break;
			}
			break;
		case TRIGGER_MODE_CC:
			triggerModeCCValue = value;
			switch (triggerModeCCValue)
			{
			case TRIGGER_MODE_LEVEL_NON_INV_VALUE:
				setMode(TriggeredOnOff::LEVEL_NON_INV);
				break;
			case TRIGGER_MODE_LEVEL_INV_VALUE:
				setMode(TriggeredOnOff::LEVEL_INV);
				break;
			case TRIGGER_MODE_EDGE_OFF_ON_VALUE:
				setMode(TriggeredOnOff::EDGE_OFF_ON);
				break;
			case TRIGGER_MODE_EDGE_ON_OFF_VALUE:
				setMode(TriggeredOnOff::EDGE_ON_OFF);
				break;
			default:
				break;
			}
			break;
		default:
			break;
	}
}

uint8_t TriggeredOnOff::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case TRIGGER_MODE_CC:
			return triggerModeCCValue;
		case TRIGGER_CONTROL_TYPE_CC:
			return controlTypeCCValue;
		default:
			return DO_NOT_SAVE_VALUE;
	}
}
