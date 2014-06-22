#ifndef __CV2__RateOfChangeLimiter__
#define __CV2__RateOfChangeLimiter__

#include <Arduino.h>

class RateOfChangeLimiter
{
public:
	RateOfChangeLimiter();
	virtual ~RateOfChangeLimiter();
	uint8_t getValue();
	void setTarget(const uint8_t n);
	void setSteps(const uint8_t n);
private:
	int16_t target;
	int16_t value;
	uint8_t numSteps;
	int16_t stepValue;
};

#endif /* defined(__CV2__RateOfChangeLimiter__) */
