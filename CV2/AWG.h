#ifndef __CV2__AWG__
#define __CV2__AWG__

#include <Arduino.h>
#include "ValueProvider.h"
#include "OnOffTriggerable.h"
#include "SourceValueProvider.h"

class AWG : public OnOffTriggerable, public ValueProvider
{
public:
	static AWG& instance(const uint8_t index);
	AWG(const uint8_t index, const uint8_t aMidiChannel);
	virtual ~AWG();
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
	typedef struct Phase_t
	{
		uint8_t endPoint;
		boolean sustain;
		unsigned long incrementUs; // the time between increments
		unsigned long startUs;
		unsigned long durationUs;
		int8_t increment; // could be negative
		uint8_t duration; // in time steps
	} Phase;

	boolean incrementValue(uint16_t& value, Phase& p);
	//unsigned long getPhaseTime(const uint16_t phaseTimeCCValue);
	void startPhase(const uint8_t phaseIndex, const unsigned long usNow);
	uint8_t midiChannel;
	uint16_t value;
	unsigned long lastValueUs;
	boolean running;
	boolean loopEnabled;
	SourceValueProvider msPerTimeStep;
	
	static const uint8_t NUM_PHASES = 8;
	Phase phases[NUM_PHASES];
	uint8_t currentPhase;
};

#endif /* defined(__CV2__AWG__) */
