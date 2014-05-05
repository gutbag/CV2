#ifndef CV2_MIDIPCListener_h
#define CV2_MIDIPCListener_h

#include <Arduino.h>

class MIDIPCListener
{
public:
	virtual void processPCMessage(const uint8_t channel,
								  const uint8_t programNumber) = 0;
};

#endif
