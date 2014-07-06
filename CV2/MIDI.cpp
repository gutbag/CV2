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
  readIndex(0),
  messageCount(0),
  lastTransmitMs(0)
{
	if (pInstance == NULL)
		pInstance = this;
	
	for (unsigned int ch=0; ch<16; ch++)
	{
		for (unsigned int i=0; i<128; i++)
		{
			ccListeners[ch][i] = NULL;
		}
	}
	
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
		if (0)
		{
			Serial.print("MIDI::loop n ");
			Serial.println(n, DEC);
		}
		
		if (millis() - lastTransmitMs > IGNORE_RX_PERIOD_MS)
		{
			int numFree = BUFFER_SIZE - writeIndex;
			
			if (n < numFree) // there's enough space in the buffer
			{
				Serial1.readBytes(&rxBuffer[writeIndex], n);
				writeIndex += n;
				uint16_t bytesConsumed = processBuffer((uint8_t*)&rxBuffer[readIndex], writeIndex - readIndex);
				readIndex += bytesConsumed;
				shuffleBuffer();
			}
			else
			{
				// TODO: not enough space in buffer - do what?
				Display::instance().set("Err!");
			}
		}
		else // ignore rx bytes - empty buffer
		{
			if (0)
			{
				Serial.print("MIDI::loop discarding ");
				Serial.println(n, DEC);
			}
			
			for (int i=0; i<n; i++)
				Serial1.read();
//			
//			char tmpBuffer[BUFFER_SIZE];
//			Serial1.readBytes(tmpBuffer, n);
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
 * Feeds MIDI messages from the given buffer to listeners. Returns the actual
 * length used.
 */
uint16_t MIDI::processBuffer(const uint8_t* buffer, const uint16_t length, const boolean transmit /*= false*/)
{
	//uint16_t numAvailable = length;
	uint16_t i = 0;
	boolean exitLoop = false;
	unsigned int messageCountBefore = messageCount;
	
	//dumpBuffer(buffer, length);
	
	while ( ! exitLoop && length - i >= 2) // at least one message in buffer
	{
//		Serial.print("length=");
//		Serial.print(length, DEC);
//		Serial.print(" writeIndex=");
//		Serial.print(writeIndex, DEC);
//		Serial.print(" readIndex=");
//		Serial.println(readIndex, DEC);
		
		const uint8_t* pBuffer = &buffer[i];
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
				if (length - i > 2)
				{
					uint8_t controllerNumber = pBuffer[1] & 0x7f;
					MIDICCListener* pListener = ccListeners[channel][controllerNumber];
					if (pListener != NULL)
					{
						if (0)
						{
							Serial.print("MIDI::processBuffer (len ");
							Serial.print(length);
							Serial.print(") CC ");
							Serial.print(channel, DEC);
							Serial.print(" ");
							Serial.print(controllerNumber, DEC);
							Serial.print(" ");
							Serial.println(pBuffer[2], DEC);
						}
						if (transmit)
							transmitCC(channel, controllerNumber, pBuffer[2]);
						else // only inc for msgs received from MIDI IN - TODO: make this better
						{
							messageCount++;
							
							if (0)
							{
								Serial.print("!");
								Serial.println(messageCount, DEC);
							}
						}
						
						// must do this after the messageCount inc in case the listener uses it
						pListener->processCCMessage(channel, controllerNumber, pBuffer[2]);
					}
					i += 3;
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
//					Serial.print(" ");
//					Serial.print(channel, DEC);
//					Serial.print(" ");
//					Serial.println(patchNumber, DEC);
					messageCount++;
					pcListener->processPCMessage(channel, patchNumber);
				}
				i += 2;
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
				//i = length;
				i+=3; // assume 3 for now - also risky
				exitLoop = true;
				break;
		}
		
		if (messageCountBefore != messageCount)
			Display::instance().flashColon();
	}
	
	return i;
}

/**
 * Move the buffer contents between readIndex and writeIndex back to the start.
 */
void MIDI::shuffleBuffer()
{
	unsigned int destIndex = 0;
	
	for (unsigned int i=readIndex; i<writeIndex; i++)
	{
		rxBuffer[destIndex] = rxBuffer[i];
		destIndex++;
	}
	
	writeIndex = writeIndex - readIndex;
	readIndex = 0;
}

unsigned int MIDI::getMessageCount(boolean reset)
{
	unsigned int count = messageCount;
	if (reset)
		messageCount = 0;
	return count;
}

void MIDI::resetMessageCount()
{
	Serial.println("MIDI::resetMessageCount");
	messageCount = 0;
}

void MIDI::transmitCC(const uint8_t channel, const uint8_t controllerNumber, const uint8_t value)
{
	if (0)
	{
		Serial.print("MIDI::transmitCC 0x");
		Serial.print(channel | MIDI_CONTROL_CHANGE, HEX);
		Serial.print(" ");
		Serial.print(controllerNumber, DEC);
		Serial.print(" ");
		Serial.println(value, DEC);
	}
	
	//if (channel == 0 && controllerNumber == 3)
	{
		Serial1.write(channel | MIDI_CONTROL_CHANGE);
		Serial1.write(controllerNumber);
		Serial1.write(value);
	}
	
	lastTransmitMs = millis();
}

// return true if the buffer is complete
boolean MIDI::getListenerSettingMessages(uint8_t* buffer, const unsigned int maxLength, unsigned int& length)
{
	length = 0;
	
	for (unsigned int ch=0; ch<16; ch++)
	{
		for (unsigned int i=0; i<128; i++)
		{
			MIDICCListener* pListener = ccListeners[ch][i];
			if (pListener != NULL)
			{
				uint8_t value = pListener->getControllerValue(i);
				if (value != DO_NOT_SAVE_VALUE)
				{
					buffer[length++] = MIDI_CONTROL_CHANGE | ch;
					buffer[length++] = i;
					buffer[length++] = value;
					
					//Serial.println(length, DEC);
					
					if (length >= maxLength - 3)
					{
						return false;
					}
				}
			}
		}
	}
	
	return true;
}
//
//boolean MIDI::getListenerSettingMessages(uint8_t* buffer, const unsigned int maxLength, unsigned int& length)
//{
//	length = 0;
//	
//	while (length < 172)
//	{
//		buffer[length++] = MIDI_CONTROL_CHANGE | 0;
//		buffer[length++] = 23;
//		buffer[length++] = 0x55;
//		
////		Serial.println(length, DEC);
//	}
//	
//	return true;
//}

void MIDI::dumpBuffer(const uint8_t* buffer, const uint16_t length)
{
	Serial.println("MIDI::dumpBuffer:");
	Serial.print("  length ");
	Serial.print(length, DEC);
	Serial.println(":");
	unsigned int i = 0;
	while (i < length)
	{
		Serial.print(buffer[i++], HEX);
		Serial.print(" ");
		Serial.print(buffer[i++], DEC);
		Serial.print(" 0x");
		Serial.print(buffer[i++], HEX);
		Serial.print(" | ");
		if (i % (3*4) == 0)
			Serial.println();
		delay(10);
	}
	Serial.println("---");
	
}
