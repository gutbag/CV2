#ifndef CV2_ValueConsumer_h
#define CV2_ValueConsumer_h

#include <Arduino.h>
#include "ValueProvider.h"

// is this class needed?
class ValueConsumer
{
	virtual void setProvider(ValueProvider& aValueProvider) = 0;
};

#endif
