#ifndef __CV2__DisplayTest__
#define __CV2__DisplayTest__

#include <Arduino.h>
#include "MIDIListener.h"

class DisplayTest : public MIDIListener
{
public:
	DisplayTest();
	virtual ~DisplayTest();
	void setup();
	void loop(unsigned long usNow);
	void enable(boolean state);
	void processMessage(const char* pMessage);
private:
	unsigned long lastCharUpdateUs;
	unsigned long lastDpUpdateUs;
	unsigned int charIndex;
	boolean enabled;
};

#endif /* defined(__CV2__DisplayTest__) */
