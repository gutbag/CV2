#ifndef __CV2__Patch__
#define __CV2__Patch__

#include "MIDIPCListener.h"
#include "MIDICCListener.h"
//#include "SwitchTrigger.h"
#include "OnOffEdgeProvider.h"

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
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
private:
	static const uint16_t HEADER_START_ADDR = 0x0; // magic number here, followed by header
	static const uint16_t PATCH_START_ADDR = 0x400; // the first patch's MIDI msgs
	static const uint16_t MAX_PATCH_SIZE = 1024; // space for the MIDI messages
	static const uint8_t MAX_PATCHES = (65536 - PATCH_START_ADDR) / MAX_PATCH_SIZE; // divide the remaining space up
	
	void readEepromHeader();
	boolean loadPatch(const uint8_t n);
	uint8_t getNextPatchNumber(const uint8_t currentPatchNumber, const boolean searchUp);
	uint8_t incPatchNumber(const uint8_t n, const boolean up);
	uint8_t getNextFreePatchNumber(const uint8_t currentPatchNumber);
	void saveHeader();
	void save();
	void copy();
	void refresh();
	void dumpPatch();
	void dumpEeprom();
	void erase(const uint8_t value);
	void initHeader();
	void dumpEEPROMHeader();
	void dumpBuffer(const uint8_t* buffer, const uint16_t length);
	uint16_t calcChecksum(const uint8_t* buffer, const unsigned int length);
	uint16_t calcHeaderChecksum();
	
	OnOffEdgeProvider downSwitchEdgeProvider;
	OnOffEdgeProvider upSwitchEdgeProvider;
	uint8_t patchNumber;
	uint8_t pendingPatchNumber;
	boolean patchChangePending;
	unsigned long patchChangeRequestUs;
	static const unsigned long PATCH_CHANGE_TIMEOUT_US = 1000000; // 1s
	
	class PatchInfo {
	public:
		PatchInfo() : numCCMessages(0), checksum(0) {}
		uint16_t numCCMessages; // not bytes, 0 if patch is unused
		uint16_t checksum;
	};
	class EEPROMHeader {
	public:
		uint16_t magicNumber;
		uint16_t checksum; // of header, excluding this checksum member
		uint8_t displayBrightness;
		PatchInfo patchInfo[MAX_PATCHES]; // index is patch number
	};
	static const uint16_t MAGIC_NUMBER = 0xf00d;
	EEPROMHeader eepromHeader;
	
	typedef enum {
		IDLE,
		WAIT_CONFIRM_SWITCH_CHANGE,
		WAIT_CONFIRM_MIDI_CHANGE,
		WAIT_CONFIRM_NEW,
		WAIT_CONFIRM_COPY
	} State;
	State state;
	
	boolean transmitMIDI;
};

#endif /* defined(__CV2__Patch__) */
