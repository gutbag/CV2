#ifndef __CV2__CVOutput__
#define __CV2__CVOutput__

#include <Arduino.h>
#include "MIDICCListener.h"

class DAC;
class ValueProvider;
class MIDI;

class CVOutput : public MIDICCListener
{
public:
	CVOutput(DAC& aDac, const uint8_t anOutput, const uint8_t aMidiChannel);
	virtual ~CVOutput();
	void setup();
	void loop(const unsigned int usNow);
	void setValueProvider(ValueProvider* aValueProvider);
	void setMinimum(const uint8_t value);
	void setMaximum(const uint8_t value);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	DAC& dac;
	uint8_t output;
	uint8_t midiChannel;
	uint8_t value;
	uint8_t minimum;
	uint8_t maximum;
	uint8_t sideChainMinimum;
	uint8_t sideChainMaximum;
	ValueProvider* pProvider;
	uint8_t sourceTypeId;
	uint16_t lastProviderValue;
	ValueProvider* pSideChainProvider;
	uint8_t sideChainSourceTypeId;
	boolean dirty;
	enum SideChainMode {MIN, MAX, RANGE};
	SideChainMode sideChainMode;
	uint8_t sideChainModeId;
};

#endif /* defined(__CV2__CVOutput__) */
