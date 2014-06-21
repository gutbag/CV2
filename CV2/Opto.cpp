#include "Opto.h"
#include "OnOffStateProvider.h"
#include "Footswitch.h"

static Opto* pInstances[4] = {NULL, NULL, NULL, NULL};

Opto& Opto::instance(const uint8_t index)
{
	return *pInstances[index];
}


Opto::Opto(const uint8_t instanceIndex, const uint8_t aPin)
: pin(aPin), onState(false)
{
	pInstances[instanceIndex] = this;
}

Opto::~Opto()
{
	
}

void Opto::setup()
{
	pinMode(pin, OUTPUT);
	onState = true; // so that off works
	off();
	
	// TODO: register with MIDI
	
	// TEMP - map to a footswitch
//	for (uint8_t i=0; i<3; i++)
//	{
//		if (pInstances[i] == this)
//		{
//			onOff.setProvider(&Footswitch::instance(i));
//			onOff.setMode(TriggeredOnOff::EDGE_OFF_ON);
//			break;
//		}
//	}
	
	onOff.setup();
}

void Opto::loop(const unsigned long usNow)
{
	onOff.loop(usNow);
	
	if (onOff.isOn())
		on();
	else
		off();
}

void Opto::on()
{
	if ( ! onState)
	{
		digitalWrite(pin, HIGH);
		onState = true;
	}
}

void Opto::off()
{
	if (onState)
	{
		digitalWrite(pin, LOW);
		onState = false;
	}
}
