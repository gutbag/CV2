#ifndef __CV2__Noise__
#define __CV2__Noise__

#include <Arduino.h>
#include "ValueProvider.h"
#include "MIDICCListener.h"

class TriggeredOnOff;

class Noise : public ValueProvider, public MIDICCListener
{
public:
	static Noise& instance(const uint8_t index);
	Noise(const uint8_t index, const uint8_t aMidiChannel);
	virtual ~Noise();
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	uint16_t value;
	unsigned long lastValueUs;
	unsigned long valueDelayUs;
	uint8_t amplitude;
	uint8_t rateCCValue;
	uint8_t amplitudeCCValue;
	TriggeredOnOff* enable;
	uint8_t midiChannel;
	uint8_t triggerInstanceCCValue;
};

#endif /* defined(__CV2__Noise__) */
