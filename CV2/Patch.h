#ifndef __CV2__Patch__
#define __CV2__Patch__

#include "MIDIListener.h"

/**
 * Knows how to read values from EEPROM and set all object settings?????
 * returns the number of bytes consumed in the EEPROM
 * Patch factory? 
 * need to store number of patches at start?
 */
class Patch : public MIDIListener
{
public:
	void processMessage(const char* pMessage);
private:
	void printControlChange(const char* pMessage);
};

#endif /* defined(__CV2__Patch__) */
