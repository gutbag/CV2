#include "EEPROM.h"
#include <Wire.h>

EEPROM EEPROM::theInstance;

EEPROM::EEPROM()
{
}

EEPROM::~EEPROM()
{
}

EEPROM& EEPROM::instance()
{
	return theInstance;
}

unsigned int EEPROM::read(const uint16_t address, uint8_t* buffer, const uint16_t n)
{
	if (address + n > sizeof(memory))
	{
		Serial.print("EEPROM::read() address 0x");
		Serial.print(address, HEX);
		Serial.println(" out of range");
	}

	for (uint16_t i=0; i<n; i++)
	{
		buffer[i] = memory[address + i];
	}
	
	return 0;
}

unsigned int EEPROM::write(const uint16_t address, uint8_t* buffer, const uint16_t n)
{
	if (address + n > sizeof(memory))
	{
		Serial.print("EEPROM::read() address 0x");
		Serial.print(address, HEX);
		Serial.println(" out of range");
	}
	
	for (uint16_t i=0; i<n; i++)
	{
		memory[address + i] = buffer[i];
	}

	return 0;
}
