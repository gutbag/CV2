#ifndef CV2_MIDICCListener_h
#define CV2_MIDICCListener_h

#include <Arduino.h>

class MIDICCListener
{
public:
	virtual void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value) = 0;
	
	/**
	 * Returns the current value of the given controller number. If the MSB is
	 * set in the return value, it should not be saved (TODO: review whether
	 * this should be controlled separately).
	 */
	virtual uint8_t getControllerValue(const uint8_t controllerNumber) = 0;
};

#endif
