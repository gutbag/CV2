#ifndef __CV2__Patch__
#define __CV2__Patch__

#include "MIDIPCListener.h"

class CVOutput;

/**
 * Knows how to read values from EEPROM and set all object settings?????
 * returns the number of bytes consumed in the EEPROM
 * Patch factory? 
 * need to store number of patches at start?
 */
class Patch : public MIDIPCListener
{
public:
	Patch();
	virtual ~Patch();
	void processPCMessage(const uint8_t channel,
								  const uint8_t programNumber);
private:
	void printControlChange(const char* pMessage);
	void handleControlChange(const char* pMessage);
	
	CVOutput* outputs;
};

#endif /* defined(__CV2__Patch__) */
