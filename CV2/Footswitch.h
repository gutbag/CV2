#ifndef __CV2__Footswitch__
#define __CV2__Footswitch__

#include <Arduino.h>
#include "Switch.h"
#include "TriggeredOnOff.h"
#include "MIDICCListener.h"

class Footswitch : public Switch, public MIDICCListener
{
public:
	static Footswitch& instance(const uint8_t index);
	Footswitch(const uint8_t instanceIndex, const uint8_t aMidiChannel, const uint8_t aSwPin, const uint8_t anLedPin);
	virtual ~Footswitch();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	void setLed(const boolean on, const boolean force=false);
	uint8_t ledPin;
	uint8_t midiChannel;
	uint8_t onOffInstanceNumber;
	TriggeredOnOff* pOnOff;
	boolean ledIsOn;
};

#endif /* defined(__CV2__Footswitch__) */
