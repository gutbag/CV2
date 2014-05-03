#ifndef __CV2__DisplayTest__
#define __CV2__DisplayTest__

#include <Arduino.h>

class DisplayTest
{
public:
	DisplayTest();
	virtual ~DisplayTest();
	void setup();
	void loop(unsigned long usNow);
	void enable(boolean state);
private:
	unsigned long lastCharUpdateUs;
	unsigned long lastDpUpdateUs;
	unsigned int charIndex;
	boolean enabled;
};

#endif /* defined(__CV2__DisplayTest__) */
