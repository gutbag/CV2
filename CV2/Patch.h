#ifndef __CV2__Patch__
#define __CV2__Patch__

#include "MIDIPCListener.h"
#include "MIDICCListener.h"
#include "SwitchTrigger.h"

class Switch;

/**
 * Knows how to read values from EEPROM and set all object settings?????
 * returns the number of bytes consumed in the EEPROM
 * Patch factory? 
 * need to store number of patches at start?
 */
class Patch : public MIDIPCListener, public MIDICCListener
{
public:
	Patch(Switch& aDownSwitch, Switch& anUpSwitch);
	virtual ~Patch();
	void processPCMessage(const uint8_t channel,
								  const uint8_t programNumber);
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	void readEepromHeader();
	void loadPatch(const uint8_t n);
	uint8_t getNextPatchNumber(const uint8_t currentPatchNumber, const boolean searchUp);
	void save();
	void copy();
	void dumpEEPROMHeader();
	
	SwitchTrigger downSwitchTrigger;
	SwitchTrigger upSwitchTrigger;
	uint8_t patchNumber;
	uint8_t pendingPatchNumber;
	static const uint8_t MAX_PATCHES = 168;
	static const uint8_t MAX_PATCH_SIZE = 384; // space for the MIDI messages
	static const uint8_t PATCH_START_ADDR = 0x400; // the first patch's MIDI msgs
	class PatchInfo {
	public:
		PatchInfo() : numCCMessages(0), checksum(0) {}
		uint8_t numCCMessages; // not bytes, 0 if patch is unused
		uint16_t checksum;
	};
	class EEPROMHeader {
	public:
		uint16_t checksum; // of header
		uint8_t displayBrightness;
		uint8_t selectedPatch;
		PatchInfo patchInfo[MAX_PATCHES]; // index is patch number
	};
	EEPROMHeader eepromHeader;
	typedef enum State {
		IDLE,
		WAIT_CONFIRM_SWITCH_CHANGE,
		WAIT_CONFIRM_MIDI_CHANGE,
		WAIT_CONFIRM_NEW,
		WAIT_CONFIRM_COPY
	};
	State state;
};

#endif /* defined(__CV2__Patch__) */
