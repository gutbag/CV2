#ifndef __CV2__Ramp__
#define __CV2__Ramp__

#include <Arduino.h>
#include "ValueProvider.h"
#include "MIDICCListener.h"

class TriggeredOnOff;

class Ramp : public ValueProvider, public MIDICCListener
{
public:
	static Ramp& instance(const uint8_t index);
	Ramp(const uint8_t index, const uint8_t aMidiChannel);
	virtual ~Ramp();
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
	void setIdleValue();
	void calcValueDelay();
	
	uint16_t value;
	boolean startAtMin;
	typedef enum {IDLE, RUNNING, FINISHED} State;
	State state;
	TriggeredOnOff* trigger;
	uint8_t directionCCValue;
	uint8_t rampTimeCCValue;
	unsigned long lastValueUs;
	unsigned long valueDelayUs;
	unsigned long startUs;
	unsigned long rampTimeUs;
	uint8_t midiChannel;
	uint8_t triggerInstanceCCValue;
};

#endif /* defined(__CV2__Ramp__) */
