#ifndef __CV2__CVOutput__
#define __CV2__CVOutput__

#include <Arduino.h>

class DAC;
class ValueProvider;

class CVOutput
{
public:
	CVOutput(DAC& aDac, const uint8_t anOutput);
	virtual ~CVOutput();
	void setup();
	void loop(const unsigned int usNow);
	void setValueProvider(ValueProvider* aValueProvider);
	void setMinimum(const uint8_t value);
	void setMaximum(const uint8_t value);
private:
	DAC& dac;
	uint8_t output;
	uint8_t value;
	uint8_t minimum;
	uint8_t maximum;
	ValueProvider* pProvider;
	uint16_t lastProviderValue;
};

#endif /* defined(__CV2__CVOutput__) */
