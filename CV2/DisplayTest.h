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
private:
	unsigned long lastUpdateUs;
	unsigned int charIndex;
};

#endif /* defined(__CV2__DisplayTest__) */
