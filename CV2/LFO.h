#ifndef __CV2__LFO__
#define __CV2__LFO__

#include "ValueProvider.h"
#include "OnOffTriggerable.h"
#include "SourceValueProvider.h"

class LFO : public OnOffTriggerable, public ValueProvider
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
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
	void addSlave(LFO* pLFO);
	void setSampleIndex(const uint16_t i); // TODO: generalise? interface class?
private:
	void setFrequencyRange(const uint8_t value);
	void setFrequency(const boolean force);
	float fMin;
	float fMax;
	float hertzPerStep;
	unsigned int sampleIndex;
	unsigned long usLastSample;
	unsigned long usBetweenSamples;
	SourceValueProvider freqStep;
	uint16_t freqStepValue;
	uint8_t freqRange;
	uint8_t midiChannel;
	LFO* slave;
	uint8_t phaseOffsetIndex; // MIDI value
};

#endif /* defined(__CV2__LFO__) */
