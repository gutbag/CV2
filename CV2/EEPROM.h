#ifndef __CV2__EEPROM__
#define __CV2__EEPROM__

#include <Arduino.h>

class EEPROM
{
public:
	static EEPROM& instance();
	
	unsigned int read(const uint16_t address, uint8_t* buffer, const uint16_t n);
	unsigned int write(const uint16_t address, uint8_t* buffer, const uint16_t n);
	
private:
	static EEPROM theInstance;
	EEPROM();
	virtual ~EEPROM();
	uint8_t memory[2048]; // TODO: temp!!!!
};

#endif /* defined(__CV2__EEPROM__) */
