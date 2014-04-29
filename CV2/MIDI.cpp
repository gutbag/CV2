#include "MIDI.h"
#include "MIDIListener.h"

//MIDI::MIDI(HardwareSerial& aPort)
//: port(aPort)
MIDI::MIDI()
: writeIndex(0),
  readIndex(0),
  pListener(NULL)
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
		int numFree = BUFFER_SIZE - writeIndex;
		
		if (n < numFree)
		{
			Serial1.readBytes(&buffer[writeIndex], n);
			writeIndex += n;
			processBuffer();
		}
		else
		{
			// not enough space in buffer - do what?
		}
	}
}

void MIDI::setListener(MIDIListener* pAListener)
{
	pListener = pAListener;
}

/**
 * Read MIDI messages from the buffer and deliver them. Shuffle the buffer
 * to make space.
 */
void MIDI::processBuffer()
{
	while (writeIndex - readIndex >= 3) // at least one message in buffer
	{
		char* pBuffer = &buffer[readIndex];
		
		switch (*pBuffer & 0xf0)
		{
			case 0xb0: // Control Change
				if (pListener != NULL)
					pListener->processMessage(pBuffer);
				readIndex += 3;
				break;
			default:
				break;
		}
	}
	
	shuffleBuffer();
}

/**
 * Move the buffer contents between readIndex and writeIndex back to the start.
 */
void MIDI::shuffleBuffer()
{
	unsigned int destIndex = 0;
	
	for (unsigned int i=readIndex; i<writeIndex; i++)
	{
		buffer[destIndex] = buffer[i];
		destIndex++;
	}
	
	writeIndex = writeIndex - readIndex;
	readIndex = 0;
}





