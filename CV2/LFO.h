#ifndef __CV2__LFO__
#define __CV2__LFO__

#include "ValueProvider.h"

class LFO : public ValueProvider
{
public:
	LFO();
	virtual ~LFO();
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();	
	void setup();
	void loop(const unsigned long usNow);
};

#endif /* defined(__CV2__LFO__) */
