#ifndef __CV2__MIDI__
#define __CV2__MIDI__

#include <Arduino.h>
#include "MIDIMessages.h"

class MIDICCListener;
class MIDIPCListener;

class MIDI
{
public:
	static MIDI& instance();
	MIDI();
	virtual ~MIDI();
	void setup();
	void loop(const unsigned long usNow);
	void setCCListener(MIDICCListener* pAListener,
					   const uint8_t channel, const uint8_t controllerNumber);
	void setPCListener(MIDIPCListener* pAListener);
	uint16_t processBuffer(const uint8_t* pBuffer, const uint16_t length);
private:
	static const unsigned int BUFFER_SIZE = 256;
	
	void processBuffer();
	void shuffleBuffer();
	
	char buffer[BUFFER_SIZE];
	unsigned int writeIndex;
	unsigned int readIndex;
	MIDICCListener* ccListeners[16][128]; // [channel][ctrl no]
	MIDIPCListener* pcListener; // ignore channel for PC
};

#endif /* defined(__CV2__MIDI__) */
