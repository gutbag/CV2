#include "MIDI.h"
#include "MIDIListener.h"

static MIDI* pInstance = NULL;

MIDI& MIDI::instance()
{
	return *pInstance;
}

MIDI::MIDI()
: writeIndex(0),
  readIndex(0)
{
	if (pInstance == NULL)
		pInstance = this;
	
	for (unsigned int ch=0; ch<16; ch++)
		for (unsigned int i=0; i<128; i++)
			listeners[ch][i] = NULL;
}

MIDI::~MIDI()
{
}

void MIDI::setup()
{
	Serial1.begin(31250);
}

/**
 * Check for new chars received, add them to the buffer and process whole messages.
 */
void MIDI::loop(const unsigned long usNow)
{
	int n = Serial1.available();
	if (n > 0)
	{
		int numFree = BUFFER_SIZE - writeIndex;
		
		if (n < numFree) // there's enough space in the buffer
		{
			Serial1.readBytes(&buffer[writeIndex], n);
			writeIndex += n;
			processBuffer();
		}
		else
		{
			// TODO: not enough space in buffer - do what?
		}
	}
}

void MIDI::setListener(MIDIListener* pAListener, const uint8_t channel, const uint8_t message, const uint8_t controllerNumber)
{
	if ((message & 0xf0) == 0xb0) // only handling Control Change currently
	{
		if (controllerNumber < 128)
		{
//			Serial.print("MIDI::setListener 0x");
//			Serial.print((unsigned long)pAListener, HEX);
//			Serial.print(" ");
//			Serial.print(channel, DEC);
//			Serial.print(" ");
//			Serial.println(controllerNumber, DEC);
			listeners[channel][controllerNumber] = pAListener;
		}
	}
}

/**
 * Read MIDI messages from the buffer and deliver them. Shuffle the buffer
 * to make space.
 */
void MIDI::processBuffer()
{
	while (writeIndex - readIndex >= 3) // at least one message in buffer
	{
//		Serial.print("writeIndex=");
//		Serial.print(writeIndex, DEC);
//		Serial.print(" readIndex=");
//		Serial.println(readIndex, DEC);

		char* pBuffer = &buffer[readIndex];
		uint8_t message = *pBuffer & 0xf0;
		uint8_t channel = *pBuffer & 0x0f;
		
//		Serial.print("MIDI::processBuffer msg 0x");
//		Serial.print(message, HEX);
//		Serial.print(" ch ");
//		Serial.println(channel, DEC);
		
		// TODO: validate channel
		
		switch (message)
		{
			case 0xb0: // Control Change
			{
				uint8_t controllerNumber = pBuffer[1] & 0x7f;
				MIDIListener* pListener = listeners[channel][controllerNumber];
				if (pListener != NULL)
				{
//					Serial.print("MIDI::processBuffer CC 0x");
//					Serial.print((unsigned long)pListener, HEX);
//					Serial.print(" ");
//					Serial.print(channel, DEC);
//					Serial.print(" ");
//					Serial.println(controllerNumber, DEC);
					pListener->processMessage(pBuffer);
				}
				readIndex += 3;
			}
				break;
			default:
				Serial.println("no listener");
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


