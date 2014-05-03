#ifndef __CV2__DAC__
#define __CV2__DAC__

#include <Arduino.h>

class DAC
{
public:
	DAC(const uint8_t aCSPin);
	virtual ~DAC();
	void setup();
	void setOutput(const uint8_t output, const uint8_t value);
	
private:
	static const uint8_t RANGE = 1;
	uint8_t csPin;
	uint8_t values[8];
};

#endif /* defined(__CV2__DAC__) */
