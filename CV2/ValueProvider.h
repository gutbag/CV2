#ifndef CV2_ValueProvider_h
#define CV2_ValueProvider_h

#include <Arduino.h>

class ValueProvider
{
public:
	virtual uint16_t getMinimum() = 0;
	virtual uint16_t getMaximum() = 0;
	virtual uint16_t getValue() = 0;
};

#endif
