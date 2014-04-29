#ifndef __CV2__MIDI__
#define __CV2__MIDI__

#include <Arduino.h>
//#include <HardwareSerial.h>

class MIDIListener;

class MIDI
{
public:
//	MIDI(HardwareSerial& aPort);
	MIDI();
	virtual ~MIDI();
	void setup();
	void loop(const unsigned long usNow);
	void setListener(MIDIListener* pAListener);
private:
	static const unsigned int BUFFER_SIZE = 256;
	
	void processBuffer();
	void shuffleBuffer();
	
//	HardwareSerial& port;
	char buffer[BUFFER_SIZE];
	unsigned int writeIndex;
	unsigned readIndex;
	MIDIListener* pListener;
};

#endif /* defined(__CV2__MIDI__) */
