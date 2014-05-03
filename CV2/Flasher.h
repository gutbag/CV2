#ifndef __CV2__Flasher__
#define __CV2__Flasher__

#include <Arduino.h>

class Flasher
{
public:
	Flasher(const unsigned long anOnTimeUs, const unsigned long aMinOffTimeUs);
	virtual ~Flasher();
	void setup();
	void loop(const unsigned long usNow);
	boolean getState() const;
	void flash();
private:
	boolean state;
	boolean flashRequest;
	unsigned long onTimeUs;
	unsigned long minOffTimeUs;
	unsigned long lastChangeUs;
};

#endif /* defined(__CV2__Flasher__) */
