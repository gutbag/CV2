#include "Patch.h"
#include "Switch.h"
#include "Display.h"
#include "MIDI.h"
#include "EEPROM.h"
#include "Errors.h"

Patch::Patch(Switch& aDownSwitch, Switch& anUpSwitch)
: downSwitchTrigger(aDownSwitch),
  upSwitchTrigger(anUpSwitch),
  patchNumber(0),
  pendingPatchNumber(0),
  state(IDLE)
{
	// write fake header info
	uint16_t checksum = 0;
	EEPROM::instance().write(0, (uint8_t*)&checksum, sizeof(checksum));
	uint8_t brightness = 127;
	EEPROM::instance().write(2, (uint8_t*)&brightness, sizeof(brightness));
	uint8_t selectedPatch = 0;
	EEPROM::instance().write(3, (uint8_t*)&selectedPatch, sizeof(selectedPatch));

	// fake info for first two patches
	uint8_t numCCMessages = 1;
	EEPROM::instance().write(4, (uint8_t*)&numCCMessages, sizeof(numCCMessages));
	EEPROM::instance().write(5, (uint8_t*)&checksum, sizeof(checksum));
	EEPROM::instance().write(7, (uint8_t*)&numCCMessages, sizeof(numCCMessages));
	EEPROM::instance().write(8, (uint8_t*)&checksum, sizeof(checksum));

	uint8_t cc0[] = {0xb0, CV_OUTPUT_SOURCE_CC, CV_OUTPUT_SOURCE_FIXED_VALUE};
	EEPROM::instance().write(PATCH_START_ADDR + MAX_PATCH_SIZE * 0, cc0, 3);
	uint8_t cc1[] = {0xb0, CV_OUTPUT_SOURCE_CC, CV_OUTPUT_SOURCE_LFO_VALUE};
	EEPROM::instance().write(PATCH_START_ADDR + MAX_PATCH_SIZE * 1, cc1, 3);
}

Patch::~Patch()
{
	
}

void Patch::processPCMessage(const uint8_t channel, const uint8_t programNumber)
{
	Serial.print("Patch::processPCMessage ch ");
	Serial.print(channel, HEX);
	Serial.print(" n ");
	Serial.println(programNumber, HEX);

	switch (channel)
	{
		case 0:
			break;
		default:
			break;
	}
}

void Patch::processCCMessage(const uint8_t channel, const uint8_t controllerNumber, const uint8_t value)
{
	Serial.print("Patch CC Message, ch: ");
	Serial.print(channel, DEC);
	Serial.print(" No: ");
	Serial.print(controllerNumber, DEC);
	Serial.print(" Val: ");
	Serial.println(value, DEC);
	
	switch (controllerNumber)
	{
		case PATCH_SAVE_CC:
			save();
			break;
		case PATCH_COPY_CC:
			copy();
			break;
		default:
			break;
	}
}

uint8_t Patch::getControllerValue(const uint8_t controllerNumber)
{
	return DO_NOT_SAVE_VALUE;
}

void Patch::setup()
{
	downSwitchTrigger.setup();
	upSwitchTrigger.setup();
	
	MIDI::instance().setPCListener(this);
	MIDI::instance().setCCListener(this, 0, PATCH_SAVE_CC);
	MIDI::instance().setCCListener(this, 0, PATCH_COPY_CC);
	
	readEepromHeader();
	loadPatch(patchNumber);
}

void Patch::loop(const unsigned long usNow)
{
	downSwitchTrigger.loop(usNow);
	boolean downPressed = downSwitchTrigger.triggered();
	upSwitchTrigger.loop(usNow);
	boolean upPressed = upSwitchTrigger.triggered();
	
	switch (state)
	{
		case IDLE:
		{
			if (downPressed && upPressed)
			{
				// TODO: could use both for something
			}
			else if (downPressed)
			{
				Serial.println("--> down");
				
				pendingPatchNumber = getNextPatchNumber(patchNumber, false);
				
				if (MIDI::instance().getMessageCount(true) > 0) // changes made, prompt for save
				{
					Display::instance().set("Sav!");
					state = WAIT_CONFIRM_SWITCH_CHANGE;
				}
				else
				{
					patchNumber = pendingPatchNumber;
					loadPatch(patchNumber);
				}
			}
			else if (upPressed)
			{
				Serial.println("--> up");
				Serial.print(MIDI::instance().getMessageCount(false), DEC);
				Serial.println(" msgs");

				pendingPatchNumber = getNextPatchNumber(patchNumber, true);

				if (MIDI::instance().getMessageCount(true) > 0) // changes made, prompt for save
				{
					Display::instance().set("Sav!");
					state = WAIT_CONFIRM_SWITCH_CHANGE;
				}
				else
				{
					patchNumber = pendingPatchNumber;
					loadPatch(patchNumber);
				}
			}
		}
			break;
		case WAIT_CONFIRM_SWITCH_CHANGE:
		{
			if (downPressed && upPressed)
			{
				// TODO: could use both for something
			}
			else if (downPressed) // i.e. confirm
			{
				patchNumber = pendingPatchNumber;
				loadPatch(patchNumber);
				state = IDLE;
			}
			else if (upPressed) // cancel
			{
				Display::instance().setPatchNumber(patchNumber);
				state = IDLE;
			}
		}
			break;
		case WAIT_CONFIRM_MIDI_CHANGE:
		{
			
		}
			break;
		case WAIT_CONFIRM_NEW:
		{
			
		}
			break;
		case WAIT_CONFIRM_COPY:
		{
			
		}
			break;
	}
}

/**
 * Finds and returns the number of the next patch that has a valid start address
 * in EEPROM.
 */
uint8_t Patch::getNextPatchNumber(const uint8_t currentPatchNumber, const boolean searchUp)
{
	uint8_t next = searchUp ? currentPatchNumber + 1 : currentPatchNumber - 1;
	
	if (0)
	{
		Serial.println("Patch::getNextPatchNumber: ");
		Serial.print("  currentPatchNumber ");
		Serial.println(currentPatchNumber, DEC);
		Serial.print("  +1 numCCMessages ");
		Serial.println(eepromHeader.patchInfo[currentPatchNumber+1].numCCMessages, DEC);
		Serial.print("  +2 numCCMessages ");
		Serial.println(eepromHeader.patchInfo[currentPatchNumber+2].numCCMessages, DEC);
	}
	
	while (eepromHeader.patchInfo[next].numCCMessages == 0)
	{
		if (searchUp)
			next++;
		else
			next--;
	}
	
	if (0)
	{
		Serial.print("returning ");
		Serial.println(next, DEC);
	}
	
	return next;
}

void Patch::readEepromHeader()
{
	uint8_t buffer[sizeof(eepromHeader)];
	
//	Serial.print("sizeof(buffer) ");
//	Serial.println(sizeof(buffer), DEC);
	
	unsigned int nRead = EEPROM::instance().read(0x0, buffer, sizeof(buffer));
	
	if (nRead != sizeof(buffer))
	{
		Display::instance().displayError(PATCH_READ_HEADER_FROM_EEPROM_FAILED);
		return;
	}
	
	//dumpBuffer(buffer, nRead);
	
	uint16_t readChecksum = *((uint16_t*)&buffer[0]);
	uint8_t displayBrightness = buffer[2];
	uint8_t selectedPatch = buffer[3];
	
	uint8_t* pBuffer = &buffer[4];
	
	for (uint8_t i=0; i<MAX_PATCHES; i++)
	{
		eepromHeader.patchInfo[i].numCCMessages = *pBuffer;
		pBuffer++;
		eepromHeader.patchInfo[i].checksum = *((uint16_t*)pBuffer);
		pBuffer += 2;
	}
	
	// now got all header info
	Display::instance().setBrightness(displayBrightness);
}

// read the given patch details from EEPROM and apply settings
// return true for OK
boolean Patch::loadPatch(const uint8_t n)
{
	if (0)
	{
		Serial.print("Patch::loadPatch ");
		Serial.print(n, HEX);
		Serial.println();
	}
	
	uint16_t lengthInBytes = eepromHeader.patchInfo[n].numCCMessages * 3;
	
	if (lengthInBytes == 0)
	{
		Display::instance().displayError(PATCH_NUMBER_INVALID);
		return false;
	}

	uint8_t buffer[MAX_PATCH_SIZE];
	uint16_t address = PATCH_START_ADDR + n * MAX_PATCH_SIZE;
	
	unsigned int nRead = EEPROM::instance().read(address, buffer, lengthInBytes);
	
	if (nRead != lengthInBytes)
	{
		Display::instance().displayError(PATCH_READ_FROM_EEPROM_FAILED);
		return false;
	}
	
	// TODO: calc and check checksum
	MIDI::instance().processBuffer(buffer, lengthInBytes, true);
	
	Display::instance().setPatchNumber(n);
	
	return true;
}

void Patch::save()
{
//	Serial.println("Patch::save");

	uint8_t buffer[MAX_PATCH_SIZE];
	
	unsigned int length = 0;
	
	boolean gotAll = MIDI::instance().getListenerSettingMessages(buffer, MAX_PATCH_SIZE, length);
	// TODO: do something with gotAll
				
	uint16_t address = PATCH_START_ADDR + patchNumber * MAX_PATCH_SIZE;
		
	EEPROM::instance().write(address, buffer, length);
	MIDI::instance().resetMessageCount();
	eepromHeader.patchInfo[patchNumber].numCCMessages = length / 3;
	
	if (0)
	{
		Serial.print("Saved ");
		Serial.print(eepromHeader.patchInfo[patchNumber].numCCMessages, DEC);
		Serial.println(" CC msgs");
	}
	
	if (0) // print buffer content
	{
		Serial.print("buffer len ");
		Serial.print(length, DEC);
		Serial.println(":");
		unsigned int i = 0;
		while (i < length)
		{
			Serial.print(buffer[i++], HEX);
			Serial.print(" ");
			Serial.print(buffer[i++], DEC);
			Serial.print(" 0x");
			Serial.print(buffer[i++], HEX);
			Serial.print(" | ");
			if (i % (3*4) == 0)
				Serial.println();
			delay(10);
		}
		Serial.println();
	}
}

void Patch::copy()
{
	
}

void Patch::dumpEEPROMHeader()
{
	Serial.println("EEPROM header:");
	Serial.print("checksum: ");
	Serial.print(eepromHeader.checksum, HEX);
	Serial.print(", displayBrightness: ");
	Serial.print(eepromHeader.displayBrightness, HEX);
	Serial.print(", selectedPatch: ");
	Serial.println(eepromHeader.selectedPatch, HEX);
	
	for (unsigned int i=0; i<MAX_PATCHES; i++)
	{
		if (eepromHeader.patchInfo[i].numCCMessages != 0)
		{
			Serial.print("[");
			Serial.print(i, DEC);
			Serial.print("] checksum: ");
			Serial.print(eepromHeader.patchInfo[i].checksum, HEX);
			Serial.print(", numCCMessages: ");
			Serial.println(eepromHeader.patchInfo[i].numCCMessages, DEC);
		}
	}
}
					 
void Patch::dumpBuffer(const uint8_t* buffer, const uint16_t length)
{
	 Serial.println("Patch::dumpBuffer:");
	 Serial.print("  length ");
	 Serial.print(length, DEC);
	 Serial.println(":");
	 unsigned int i = 0;
	 while (i < length)
	 {
		 Serial.print(buffer[i++], HEX);
		 Serial.print(" ");
		 Serial.print(buffer[i++], DEC);
		 Serial.print(" 0x");
		 Serial.print(buffer[i++], HEX);
		 Serial.print(" | ");
		 if (i % 16 == 0)
			 Serial.println();
		 delay(10);
	 }
	 Serial.println("---");
}
					 
