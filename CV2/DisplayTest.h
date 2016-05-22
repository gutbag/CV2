#ifndef __CV2__DisplayTest__
#define __CV2__DisplayTest__

#include <Arduino.h>
#include "MIDICCListener.h"

class DisplayTest : public MIDICCListener
{
public:
	DisplayTest();
	virtual ~DisplayTest();
	void setup();
	void loop(unsigned long usNow);
	void enable(boolean state);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
private:
	unsigned long lastCharUpdateUs;
	unsigned long lastDpUpdateUs;
	unsigned int charIndex;
	boolean enabled;
};

#endif /* defined(__CV2__DisplayTest__) */
