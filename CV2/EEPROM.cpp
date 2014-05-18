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
	return 0;
}

unsigned int EEPROM::write(const uint16_t address, uint8_t* buffer, const uint16_t n)
{
	return 0;
}
