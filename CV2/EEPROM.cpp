#include "EEPROM.h"
#include <Wire.h>

#if not defined RAM_EEPROM
#define EEPROM_WP 23
static const uint8_t DEVICE_ADDRESS = 0x50;
#define I2C_EEPROM_TIMEOUT 1000
#define I2C_WRITEDELAY 5000
static const uint8_t CHUNK_SIZE = 128;
#endif

static EEPROM* pInstance = NULL;

EEPROM::EEPROM()
#if not defined RAM_EEPROM
: lastWriteUs(0)
#endif
{
	pInstance = this;
	
#if defined RAM_EEPROM
	for (uint16_t i=0; i<sizeof(memory); i++)
	{
		memory[i] = 0;
	}
#endif
}

EEPROM::~EEPROM()
{
}

EEPROM& EEPROM::instance()
{
	return *pInstance;
}

#if defined RAM_EEPROM
uint16_t EEPROM::read(const uint16_t address, uint8_t* buffer, const uint16_t n)
{
//	Serial.print("EEPROM::read length ");
//	Serial.println(n, DEC);

	if (address + n > sizeof(memory))
	{
		Serial.print("EEPROM::read() address 0x");
		Serial.print(address, HEX);
		Serial.println(" out of range");
		return 0;
	}

	for (uint16_t i=0; i<n; i++)
	{
		buffer[i] = memory[address + i];
	}
	
	return n;
}

uint16_t EEPROM::write(const uint16_t address, uint8_t* buffer, const uint16_t n)
{
	if (address + n > sizeof(memory))
	{
		Serial.print("EEPROM::read() address 0x");
		Serial.print(address, HEX);
		Serial.println(" out of range");
		return 0;
	}
	
	for (uint16_t i=0; i<n; i++)
	{
		memory[address + i] = buffer[i];
	}

	return n;
}

void EEPROM::dump()
{
	Serial.println("EEPROM::dumpBuffer:");
	Serial.print("  length ");
	Serial.print(sizeof(memory), DEC);
	Serial.println(":");
	unsigned int i = 0;
	//while (i < sizeof(memory))
	while (i < 0x400)
	{
		Serial.print(memory[i++], HEX);
		Serial.print(" ");
		if (i % 16 == 0)
		{
			Serial.println();
			Serial.print("0x");
			Serial.print(i, HEX);
			Serial.print(" | ");
		}
		delay(10);
	}
	Serial.println("---");
}

#else // not RAM EEPROM

void EEPROM::setup()
{
	pinMode(EEPROM_WP, OUTPUT);
	Wire.begin();
	writeEnable(false);
}

void EEPROM::writeEnable(const boolean enabled)
{
	if (enabled)
		digitalWrite(EEPROM_WP, LOW);
	else
		digitalWrite(EEPROM_WP, HIGH);
}

void EEPROM::waitEEReady()
{
    // Wait until EEPROM gives ACK again.
    // this is a bit faster than the hardcoded 5 milli
    while ((micros() - lastWriteUs) <= I2C_WRITEDELAY)
    {
        Wire.beginTransmission(DEVICE_ADDRESS);
        int x = Wire.endTransmission();
        if (x == 0) break;
    }
}


int EEPROM::writeChunk(uint16_t address, uint8_t* buffer, uint8_t length)
{
	if (1)
	{
		Serial.print("EEPROM::writeChunk to 0x");
		Serial.print(address, HEX);
		Serial.print(" len ");
		Serial.println(length, DEC);
	}
	
	if (length > CHUNK_SIZE)
		return 0;
	
    waitEEReady();
    
    //delay(100);
	
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write((int)(address >> 8));
    Wire.write((int)(address & 0xFF));
    for (uint8_t cnt = 0; cnt < length; cnt++)
		Wire.write(buffer[cnt]);
	
    int rv = Wire.endTransmission();
    lastWriteUs = micros();
    
    if (rv != 0)
    {
		Serial.print("write rv: ");
		Serial.println(rv, DEC);
		rv = 0;  // force good value for now
    }
    return rv;
}

uint16_t EEPROM::write(const uint16_t address, uint8_t* buffer, const uint16_t length)
{
	uint16_t numWritten = 0;
	
	while (numWritten < length)
	{
		uint16_t lengthThisWrite = length - numWritten;
		if (lengthThisWrite > CHUNK_SIZE)
			lengthThisWrite = CHUNK_SIZE;
		
		int status = writeChunk(address + numWritten, &buffer[numWritten], lengthThisWrite);
		
		if (status == 0)
			numWritten += lengthThisWrite;
		else
			break;
	}
	
	return numWritten;
}

// reads a max of 128 bytes, returns num read
uint8_t EEPROM::readChunk(uint16_t address, uint8_t* buffer, uint8_t length)
{
	if (0)
	{
		Serial.print("EEPROM::readChunk from 0x");
		Serial.print(address, HEX);
		Serial.print(" len ");
		Serial.println(length, DEC);
	}
	
	if (length > CHUNK_SIZE)
		return 0;
	
    waitEEReady();
	
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write((int)(address >> 8));
    Wire.write((int)(address & 0xFF));
    int rv = Wire.endTransmission();
	//    if (rv != 0) // TODO: get working Due Wire lib
	//    {
	//      Serial.print("rv: ");
	//      Serial.println(rv, DEC);
	//      return 0;  // error
	//    }
	
    Wire.requestFrom(DEVICE_ADDRESS, length);
    uint8_t cnt = 0;
    uint32_t before = millis();
    while ((cnt < length) && ((millis() - before) < I2C_EEPROM_TIMEOUT))
    {
		if (Wire.available())
		{
			buffer[cnt++] = Wire.read();
			//Serial.print("did a Wire.read()");
		}
    }
	
    return cnt;
}

uint16_t EEPROM::read(const uint16_t address, uint8_t* buffer, const uint16_t length)
{
	uint16_t numRead = 0;
	
	while (numRead < length)
	{
		uint16_t lengthThisRead = length - numRead;
		if (lengthThisRead > CHUNK_SIZE)
			lengthThisRead = CHUNK_SIZE;
		
		//unsigned long before = micros();
		
		uint16_t n = readChunk(address + numRead, &buffer[numRead], lengthThisRead);
		
//		unsigned long after = micros();
//		Serial.print(n, DEC);
//		Serial.print(" bytes readChunk took ");
//		Serial.println(after - before, DEC);
		
		if (n == lengthThisRead)
			numRead += n;
		else
			break;
	}
	
	return numRead;
}

void EEPROM::dump()
{
	Serial.println("EEPROM::dump");
}

#endif
