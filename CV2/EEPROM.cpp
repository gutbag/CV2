#include "EEPROM.h"
#include <Wire.h>

static EEPROM* pInstance = NULL;

EEPROM::EEPROM()
{
	pInstance = this;
	
	for (uint16_t i=0; i<sizeof(memory); i++)
	{
		memory[i] = 0;
	}
}

EEPROM::~EEPROM()
{
}

EEPROM& EEPROM::instance()
{
	return *pInstance;
}

unsigned int EEPROM::read(const uint16_t address, uint8_t* buffer, const uint16_t n)
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

unsigned int EEPROM::write(const uint16_t address, uint8_t* buffer, const uint16_t n)
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
