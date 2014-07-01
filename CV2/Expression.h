#ifndef __CV2__Expression__
#define __CV2__Expression__

#include <Arduino.h>
#include "ValueProvider.h"
#include "MIDICCListener.h"

class TriggeredOnOff;

class Expression : public ValueProvider, public MIDICCListener
{
public:
	static Expression& instance(const uint8_t index);
	Expression(const uint8_t aPin, const uint8_t index, const uint8_t aMidiChannel);
	virtual ~Expression();
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
	uint8_t pin;
	uint16_t value;
	TriggeredOnOff* enable;
	uint8_t midiChannel;
	uint8_t triggerInstanceCCValue;
};

#endif /* defined(__CV2__Expression__) */
