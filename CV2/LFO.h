#ifndef __CV2__LFO__
#define __CV2__LFO__

#include "ValueProvider.h"
#include "MIDIListener.h"

class LFO : public ValueProvider, public MIDIListener
{
public:
	static LFO& instance();
	LFO();
	virtual ~LFO();
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();	
	void setup();
	void loop(const unsigned long usNow);
	void setFrequency(const unsigned long f);
	void processMessage(const char* pMessage);
private:
	static const float fMin;
	static const float fMax;
	static const float hertzPerStep;
	unsigned int sampleIndex;
	unsigned long usLastSample;
	unsigned long usBetweenSamples;
};

#endif /* defined(__CV2__LFO__) */
