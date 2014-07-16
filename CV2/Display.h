#include <Arduino.h>
#include "Flasher.h"
#include "MIDICCListener.h"

class Display : public MIDICCListener
{
public:
	static Display& instance();
	Display();
	virtual ~Display() {}
	void setup();
	void loop(const unsigned long usNow);
	void setBrightness(const uint8_t value);
	void set(const char* s);
	void setPatchNumber(const uint8_t n);
	void displayError(const uint8_t errnum);
	void displayNumber(const uint16_t n);
	void setDecimalPoint(const unsigned int which, const boolean state);
    void setColon(const boolean state);
	void flashColon();
	void setApostophe(const boolean state);
	void flashApostrophe();
	void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
	void clear();
    
private:
	const char toHexDigit(const uint8_t n);
	byte charMap[128]; // TODO: make const?
	void setDigit(const uint8_t digitIndex, const uint8_t value);
	byte digits[4];
	uint8_t digitIndex;
	unsigned long usLastChange;
	Flasher colonFlasher;
	Flasher apostropheFlasher;
};