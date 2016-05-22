#ifndef __CV2__EnvelopeState__
#define __CV2__EnvelopeState__

#include <Arduino.h>
#include "OnOffStateProvider.h"
#include "OnOffTriggerable.h"

class EnvelopeFollower;

class EnvelopeState : public OnOffTriggerable
{
public:
	EnvelopeState(EnvelopeFollower& anEnv);
	virtual ~EnvelopeState();
	void setup();
	void loop(const unsigned long usNow);
	boolean isOn();
	void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value);
	
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
private:
	EnvelopeFollower& env;
	uint16_t lowHighThreshold;
	uint16_t highLowThreshold;
	uint8_t sensitivity;
	uint8_t changeStateCount;
	
//	unsigned long stableSinceUs;
//	boolean lastOnState;

	boolean onState;
};

#endif /* defined(__CV2__EnvelopeState__) */
