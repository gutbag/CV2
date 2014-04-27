#ifndef __CV2__CVOutput__
#define __CV2__CVOutput__

#include <Arduino.h>

class DAC;

class CVOutput
{
public:
	CVOutput(DAC& aDac, const uint8_t anOutput);
	virtual ~CVOutput();
	void setup();
	void loop(const unsigned int usNow);
private:
	DAC& dac;
	uint8_t output;
	uint8_t value;
};

#endif /* defined(__CV2__CVOutput__) */
