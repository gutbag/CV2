#ifndef SoftMIDIOut_cpp
#define SoftMIDIOut_cpp

#include <Arduino.h>

class SoftMIDIOut
{
public:
	SoftMIDIOut();
	virtual ~SoftMIDIOut();
	void setup();
	void loop(const unsigned long usNow);
private:
	unsigned long lastEdgeUs;
	int pinState;
};

#endif
