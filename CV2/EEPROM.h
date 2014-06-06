#ifndef __CV2__EEPROM__
#define __CV2__EEPROM__

#include <Arduino.h>

class EEPROM
{
public:
	static EEPROM& instance();
	
	EEPROM();
	virtual ~EEPROM();
	void setup();
	void writeEnable(const boolean enabled);
	uint16_t read(const uint16_t address, uint8_t* buffer, const uint16_t n);
	uint16_t write(const uint16_t address, uint8_t* buffer, const uint16_t n);

	void dump();

private:
#if defined RAM_EEPROM
	uint8_t memory[2048]; // TODO: temp!!!!
#else
	uint8_t readChunk(const uint16_t address, uint8_t* buffer, const uint8_t n);
	int writeChunk(const uint16_t address, uint8_t* buffer, const uint8_t n);
	void waitEEReady();
	
	unsigned long lastWriteUs;
#endif
};

#endif /* defined(__CV2__EEPROM__) */
