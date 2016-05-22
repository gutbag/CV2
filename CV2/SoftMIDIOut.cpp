#include "SoftMIDIOut.h"

SoftMIDIOut::SoftMIDIOut()
: lastEdgeUs(0),
  pinState(HIGH)
{
	
}

SoftMIDIOut::~SoftMIDIOut()
{
	
}

void SoftMIDIOut::setup()
{
	pinMode(16, OUTPUT);
	digitalWrite(16, HIGH);
}

void SoftMIDIOut::loop(const unsigned long usNow)
{
	//if (usNow - lastEdgeUs >= 32)
	{
		pinState = (pinState == LOW ? HIGH : LOW);
		digitalWrite(16, pinState);
		lastEdgeUs = usNow;
	}
}
