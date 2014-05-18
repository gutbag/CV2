#include "MIDI.h"
#include "MIDICCListener.h"
#include "MIDIPCListener.h"
#include "Display.h"

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
			ccListeners[ch][i] = NULL;
	
	pcListener = NULL;
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
			Display::instance().set("Err!");
		}
	}
}

void MIDI::setCCListener(MIDICCListener* pAListener, const uint8_t channel, const uint8_t controllerNumber)
{
	if (controllerNumber < 128)
	{
//		Serial.print("MIDI::setCCListener 0x");
//		Serial.print((unsigned long)pAListener, HEX);
//		Serial.print(" ");
//		Serial.print(channel, DEC);
//		Serial.print(" ");
//		Serial.println(controllerNumber, DEC);
		ccListeners[channel][controllerNumber] = pAListener;
	}
}

void MIDI::setPCListener(MIDIPCListener* pAListener)
{
	//		Serial.print("MIDI::setPCListener 0x");
	//		Serial.println((unsigned long)pAListener, HEX);
	pcListener = pAListener;
}

/**
 * Read MIDI messages from the buffer and deliver them. Shuffle the buffer
 * to make space.
 */
void MIDI::processBuffer()
{
	unsigned int numAvailable = writeIndex - readIndex;
	boolean exitLoop = false;
	
	while ( ! exitLoop && numAvailable >= 2) // at least one message in buffer
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
			case MIDI_CONTROL_CHANGE:
				if (numAvailable > 2)
				{
					uint8_t controllerNumber = pBuffer[1] & 0x7f;
					MIDICCListener* pListener = ccListeners[channel][controllerNumber];
					if (pListener != NULL)
					{
//						Serial.print("MIDI::processBuffer CC 0x");
//						Serial.print((unsigned long)pListener, HEX);
//						Serial.print(" ");
//						Serial.print(channel, DEC);
//						Serial.print(" ");
//						Serial.println(controllerNumber, DEC);
						pListener->processCCMessage(channel, controllerNumber, pBuffer[2]);
					}
					readIndex += 3;
				}
				else // not enough bytes for value
				{
					exitLoop = true;
				}
				break;
			case MIDI_PROGRAM_CHANGE:
			{
				uint8_t patchNumber = pBuffer[1] & 0x7f;
				if (pcListener != NULL)
				{
					//					Serial.print("MIDI::processBuffer PC 0x");
					//					Serial.print((unsigned long)pListener, HEX);
					//					Serial.print(" ");
					//					Serial.print(channel, DEC);
					//					Serial.print(" ");
					//					Serial.println(patchNumber, DEC);
					pcListener->processPCMessage(channel, patchNumber);
				}
				readIndex += 2;
			}
				break;
			default:
				Display::instance().set("LIS?");

				Serial.print("no listener ");
				Serial.print(pBuffer[0], HEX);
				Serial.print(" ");
				Serial.print(pBuffer[1], HEX);
				Serial.print(" ");
				Serial.print(pBuffer[2], HEX);
				Serial.print(" r ");
				Serial.print(readIndex, DEC);
				Serial.print(" w ");
				Serial.println(writeIndex, DEC);
				
				// discard the rest - TODO: risky
				readIndex = 0;
				writeIndex = 0;
				break;
		}
		
		Display::instance().flashColon();
		
		numAvailable = writeIndex - readIndex;
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
