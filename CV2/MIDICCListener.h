#ifndef CV2_MIDICCListener_h
#define CV2_MIDICCListener_h

#include <Arduino.h>

class MIDICCListener
{
public:
	virtual void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value) = 0;
};

#endif
