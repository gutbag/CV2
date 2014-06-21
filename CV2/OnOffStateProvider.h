#ifndef CV2_OnOffStateProvider_h
#define CV2_OnOffStateProvider_h

#include <Arduino.h>

class OnOffStateProvider
{
public:
	virtual boolean isOn() = 0;
};

#endif
