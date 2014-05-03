#include "DAC.h"
#include <SPI.h>

DAC::DAC(const uint8_t aCSPin)
: csPin(aCSPin)
{
	for (uint8_t i=0; i<8; i++)
		values[i] = 1; // so that setup can set the 1st value to 0
}

DAC::~DAC()
{
	
}

void DAC::setup()
{
	SPI.begin(csPin);
	SPI.setClockDivider(csPin, 84);
	SPI.setDataMode(csPin, SPI_MODE3);
	SPI.setBitOrder(csPin, MSBFIRST);

	for (uint8_t i=0; i<8; i++)
		setOutput(i, 0);
}

void DAC::setOutput(const uint8_t output, const uint8_t newValue)
{
	if (output < 8)
	{
		if (newValue != values[output])
		{
			uint8_t msb = (output << 1) | RANGE;
			SPI.transfer(csPin, msb, SPI_CONTINUE);
			SPI.transfer(csPin, newValue);
			values[output] = newValue;
		}
	}
	// TODO: else display error with code?
}
