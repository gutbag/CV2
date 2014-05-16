#ifndef __CV2__Patch__
#define __CV2__Patch__

#include "MIDIPCListener.h"
#include "SwitchTrigger.h"

class Switch;

/**
 * Knows how to read values from EEPROM and set all object settings?????
 * returns the number of bytes consumed in the EEPROM
 * Patch factory? 
 * need to store number of patches at start?
 */
class Patch : public MIDIPCListener
{
public:
	Patch(Switch& aDownSwitch, Switch& anUpSwitch);
	virtual ~Patch();
	void processPCMessage(const uint8_t channel,
								  const uint8_t programNumber);
	void setup();
	void loop(const unsigned long usNow);
private:
	SwitchTrigger downSwitchTrigger;
	SwitchTrigger upSwitchTrigger;
	uint8_t patchNumber;
};

#endif /* defined(__CV2__Patch__) */
