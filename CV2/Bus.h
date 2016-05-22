#ifndef __CV2__Bus__
#define __CV2__Bus__

#include <Arduino.h>
#include "OnOffTriggerable.h"
#include "ValueProvider.h"
#include "SourceValueProvider.h"

class Bus : public OnOffTriggerable, public ValueProvider
{
public:
	static Bus& instance(const uint8_t index);
	Bus(const uint8_t index, const uint8_t aMidiChannel);
	virtual ~Bus();
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
private:
	ValueProvider* getSource(const uint8_t sourceId);
	uint8_t midiChannel;
	uint8_t source1CCValue;
	uint8_t source2CCValue;
	SourceValueProvider mixValue;
	ValueProvider* pSource1;
	ValueProvider* pSource2;
	uint16_t value;
};

#endif /* defined(__CV2__Bus__) */
