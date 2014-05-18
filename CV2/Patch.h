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
	void readEepromHeader();
	void loadPatch(const uint8_t n);
	SwitchTrigger downSwitchTrigger;
	SwitchTrigger upSwitchTrigger;
	uint8_t patchNumber;
	class PatchInfo {
	public:
		PatchInfo() : address(0) {}
		uint16_t address;
	};
	PatchInfo patchInfo[256]; // index is patch number
};

#endif /* defined(__CV2__Patch__) */
