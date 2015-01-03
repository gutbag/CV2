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
	void setAlternateDisplay(const char* s); // \0 in any char position to disable alt display
	void setDigit(const unsigned int which, const char c);
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
	
	// TODO - should this be here?
	// populate s with the string to represent the given value n
	static void format(const uint16_t n, char* s);
    
private:
	const char toHexDigit(const uint8_t n);
	byte charMap[128]; // TODO: make const?
	void writeDigit(const uint8_t digitIndex, const uint8_t value);
	static const uint8_t MAIN = 0;
	static const uint8_t ALT = 1;
	byte digits[4][2]; // [digit index][MAIN/ALT]
	uint8_t digitIndex;
	uint8_t mainAltIndex;
	bool altSet;
	unsigned long usLastChange;
	unsigned long usLastMainAltSwitch;
	Flasher colonFlasher;
	Flasher apostropheFlasher;
};