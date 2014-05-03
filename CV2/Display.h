#include <Arduino.h>
#include "Flasher.h"

class Display
{
public:
	static Display& instance();
	Display();
	virtual ~Display() {}
	void setup();
	void loop(const unsigned long usNow);
	void set(const char* s);
	void setDecimalPoint(const unsigned int which, const boolean state);
    void setColon(const boolean state);
	void flashColon();
	void setApostophe(const boolean state);
	void flashApostrophe();
    
private:
	byte charMap[128]; // TODO: make const?
	void setDigit(const uint8_t digitIndex, const byte value);
	byte digits[4];
	uint8_t digitIndex;
	unsigned long usLastChange;
	Flasher colonFlasher;
	Flasher apostropheFlasher;
};