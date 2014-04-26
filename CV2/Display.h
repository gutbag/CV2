#include <Arduino.h>

class Display
{
public:
	Display();
	virtual ~Display() {}
	void setup();
	void loop(const unsigned long usNow);
	void set(const char* s);
    void setColon(const boolean state);
	void setApostophe(const boolean state);
    
private:
	byte charMap[128]; // TODO: make const?
	void setDigit(const uint8_t digitIndex, const byte value);
	byte digits[4];
	uint8_t digitIndex;
	unsigned long usLastChange;
};