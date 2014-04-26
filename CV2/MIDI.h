#ifndef __CV2__MIDI__
#define __CV2__MIDI__

#include <Arduino.h>
//#include <HardwareSerial.h>

class MIDI
{
public:
//	MIDI(HardwareSerial& aPort);
	MIDI();
	virtual ~MIDI();
	void setup();
	void loop(const unsigned long usNow);
private:
//	HardwareSerial& port;
};

#endif /* defined(__CV2__MIDI__) */
