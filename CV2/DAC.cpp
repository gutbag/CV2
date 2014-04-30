#include "DAC.h"
#include <SPI.h>

DAC::DAC(const uint8_t aCSPin)
: csPin(aCSPin)
{
	
}

DAC::~DAC()
{
	
}

void DAC::setup()
{
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);
//	SPI.begin();
//	SPI.setClockDivider(84);
//	SPI.setDataMode(SPI_MODE3);
//	SPI.setBitOrder(MSBFIRST);
}

void DAC::setOutput(const uint8_t output, const uint8_t value)
{
//	uint8_t msb = (output << 1) | RANGE;
//	SPI.transfer(csPin, msb, SPI_CONTINUE);
//	SPI.transfer(csPin, value);
}