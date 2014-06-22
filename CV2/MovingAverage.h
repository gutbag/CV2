#ifndef __CV2__MovingAverage__
#define __CV2__MovingAverage__

#include <Arduino.h>

class MovingAverage
{
public:
	MovingAverage();
	virtual ~MovingAverage();
	void setDepth(const uint8_t n);
	void addValue(const uint8_t n);
	uint8_t getAverage();
private:
	static const uint8_t MAX_DEPTH = 20;
	uint8_t values[MAX_DEPTH];
	uint8_t depth;
};

#endif /* defined(__CV2__MovingAverage__) */
