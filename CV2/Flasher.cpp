#include "Flasher.h"

Flasher::Flasher(const unsigned long anOnTimeUs, const unsigned long aMinOffTimeUs)
: state(false),
  flashRequest(false),
  onTimeUs(anOnTimeUs),
  minOffTimeUs(aMinOffTimeUs),
  lastChangeUs(0)
{
}

Flasher::~Flasher()
{
}

void Flasher::setup()
{
}

void Flasher::loop(const unsigned long usNow)
{
	if (state) // on
	{
		if (usNow - lastChangeUs >= onTimeUs) // time to go off
		{
			state = false;
			lastChangeUs = usNow;
		}
	}
	else // off
	{
		if (flashRequest && usNow - lastChangeUs >= minOffTimeUs) // time to go on
		{
			state = true;
			flashRequest = false;
			lastChangeUs = usNow;
		}
	}
}

boolean Flasher::getState() const
{
	return state;
}

void Flasher::flash()
{
	flashRequest = true;
}
