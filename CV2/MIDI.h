#ifndef __CV2__MIDI__
#define __CV2__MIDI__

#include <Arduino.h>
//#include <HardwareSerial.h>
#include "MIDIMessages.h"

class MIDIListener;

class MIDI
{
public:
	static MIDI& instance();
//	MIDI(HardwareSerial& aPort);
	MIDI();
	virtual ~MIDI();
	void setup();
	void loop(const unsigned long usNow);
	void setCCListener(MIDIListener* pAListener,
					   const uint8_t channel, const uint8_t controllerNumber);
private:
	static const unsigned int BUFFER_SIZE = 256;
	
	void processBuffer();
	void shuffleBuffer();
	
//	HardwareSerial& port;
	char buffer[BUFFER_SIZE];
	unsigned int writeIndex;
	unsigned readIndex;
	MIDIListener* listeners[16][128]; // [channel][ctrl no]
};

#endif /* defined(__CV2__MIDI__) */
