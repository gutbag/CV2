#ifndef __CV2__EnvelopeFollower__
#define __CV2__EnvelopeFollower__

#include <Arduino.h>
#include "ValueProvider.h"
#include "MIDICCListener.h"

class EnvelopeFollower : public ValueProvider, public MIDICCListener
{
public:
	static EnvelopeFollower& instance();

	EnvelopeFollower(const int anEnvPin);
	virtual ~EnvelopeFollower();
	void setup();
	void loop(const unsigned long usNow);
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	static const unsigned long SAMPLE_PERIOD_US = 1000;
	static EnvelopeFollower* theInstance;
	int envPin;
	uint16_t value;
	unsigned long lastSampleUs;
};

#endif /* defined(__CV2__EnvelopeFollower__) */
