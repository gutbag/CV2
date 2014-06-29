#ifndef __CV2__LFO__
#define __CV2__LFO__

#include "ValueProvider.h"
#include "MIDICCListener.h"

class TriggeredOnOff;

class LFO : public ValueProvider, public MIDICCListener
{
public:
	static LFO& instance(const uint8_t index);
	LFO(const uint8_t index, const uint8_t aMidiChannel);
	virtual ~LFO();
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();	
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	void setFrequencyRange(const uint8_t value);
	void setFrequency(const uint8_t aFreqStep);
	float fMin;
	float fMax;
	float hertzPerStep;
	unsigned int sampleIndex;
	unsigned long usLastSample;
	unsigned long usBetweenSamples;
	uint8_t freqStep;
	uint8_t freqRange;
	TriggeredOnOff* enable;
	uint8_t midiChannel;
};

#endif /* defined(__CV2__LFO__) */
