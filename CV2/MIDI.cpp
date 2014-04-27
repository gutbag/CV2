#include "MIDI.h"

//MIDI::MIDI(HardwareSerial& aPort)
//: port(aPort)
MIDI::MIDI()
{
	
}

MIDI::~MIDI()
{
	
}

void MIDI::setup()
{
	Serial1.begin(31250);
}

void MIDI::loop(const unsigned long usNow)
{
	int n = Serial1.available();
	if (n > 0)
	{
//		if enough space left in buffer, read the bytes
//		else, shuffle down
	}
}
