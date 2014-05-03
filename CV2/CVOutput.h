#ifndef __CV2__CVOutput__
#define __CV2__CVOutput__

#include <Arduino.h>
#include "MIDIListener.h"

class DAC;
class ValueProvider;
class MIDI;

class CVOutput : public MIDIListener
{
public:
	CVOutput(DAC& aDac, const uint8_t anOutput);
	virtual ~CVOutput();
	void setup();
	void loop(const unsigned int usNow);
	void setValueProvider(ValueProvider* aValueProvider);
	void setMinimum(const uint8_t value);
	void setMaximum(const uint8_t value);
	void processMessage(const char* pMessage);
private:
	DAC& dac;
	uint8_t output;
	uint8_t value;
	uint8_t minimum;
	uint8_t maximum;
	ValueProvider* pProvider;
	uint16_t lastProviderValue;
	boolean dirty;
};

#endif /* defined(__CV2__CVOutput__) */
