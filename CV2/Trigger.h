#ifndef CV2_Trigger_h
#define CV2_Trigger_h

#include <Arduino.h>

class Trigger
{
public:
	virtual boolean triggered() = 0;
};

#endif
