#ifndef __CV2__Expression__
#define __CV2__Expression__

#include <Arduino.h>
#include "ValueProvider.h"

class Expression : public ValueProvider
{
public:
	static Expression& instance(const uint8_t index);
	Expression(const uint8_t aPin, const uint8_t index);
	virtual ~Expression();
	virtual uint16_t getMinimum();
	virtual uint16_t getMaximum();
	virtual uint16_t getValue();
	void setup();
	void loop(const unsigned long usNow);
private:
	uint8_t pin;
	uint16_t value;
};

#endif /* defined(__CV2__Expression__) */
