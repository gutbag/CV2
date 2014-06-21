#ifndef __CV2__Freeze__
#define __CV2__Freeze__

#include <Arduino.h>
#include "TriggeredOnOff.h"
#include "MIDICCListener.h"

class Freeze : public MIDICCListener
{
public:
	Freeze(const uint8_t aPin, const uint8_t aMidiChannel);
	virtual ~Freeze();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	void on();
	void off();
	
	uint8_t pin;
	uint8_t midiChannel;
	uint8_t onState;
	TriggeredOnOff onOff;
	uint8_t triggerModeCCValue;
	uint8_t controlTypeCCValue;
};

#endif /* defined(__CV2__Freeze__) */
