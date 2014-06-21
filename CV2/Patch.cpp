#include "Patch.h"
#include "Switch.h"
#include "Display.h"
#include "MIDI.h"
#include "EEPROM.h"
#include "Errors.h"

Patch::Patch(Switch& aDownSwitch, Switch& anUpSwitch)
//: downSwitchTrigger(aDownSwitch),
//upSwitchTrigger(anUpSwitch),
: downSwitchEdgeProvider(&aDownSwitch),
  upSwitchEdgeProvider(&anUpSwitch),
  patchNumber(0),
  pendingPatchNumber(0),
  state(IDLE)
{
	// write fake header info
//	uint16_t checksum = 0;
//	EEPROM::instance().write(0, (uint8_t*)&checksum, sizeof(checksum));
//	uint8_t brightness = 127;
//	EEPROM::instance().write(2, (uint8_t*)&brightness, sizeof(brightness));
//	uint8_t selectedPatch = 0;
//	EEPROM::instance().write(3, (uint8_t*)&selectedPatch, sizeof(selectedPatch));
//
//	// fake info for first two patches
//	uint8_t numCCMessages = 1;
//	EEPROM::instance().write(4, (uint8_t*)&numCCMessages, sizeof(numCCMessages));
//	EEPROM::instance().write(5, (uint8_t*)&checksum, sizeof(checksum));
//	EEPROM::instance().write(7, (uint8_t*)&numCCMessages, sizeof(numCCMessages));
//	EEPROM::instance().write(8, (uint8_t*)&checksum, sizeof(checksum));
//
//	uint8_t cc0[] = {0xb0, CV_OUTPUT_SOURCE_CC, CV_OUTPUT_SOURCE_FIXED_VALUE};
//	EEPROM::instance().write(PATCH_START_ADDR + MAX_PATCH_SIZE * 0, cc0, 3);
//	uint8_t cc1[] = {0xb0, CV_OUTPUT_SOURCE_CC, CV_OUTPUT_SOURCE_LFO_VALUE};
//	EEPROM::instance().write(PATCH_START_ADDR + MAX_PATCH_SIZE * 1, cc1, 3);
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
		case ERASE_PATCH:
			erase(value);
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
//	downSwitchTrigger.setup();
//	upSwitchTrigger.setup();
	downSwitchEdgeProvider.setup();
	upSwitchEdgeProvider.setup();
	
	MIDI::instance().setPCListener(this);
	MIDI::instance().setCCListener(this, 0, PATCH_SAVE_CC);
	MIDI::instance().setCCListener(this, 0, PATCH_COPY_CC);
	MIDI::instance().setCCListener(this, 0, ERASE_PATCH);
	
	initHeader();
	
//	downSwitchTrigger.loop(usNow);
//	boolean downPressed = downSwitchTrigger.triggered();
//	upSwitchTrigger.loop(usNow);
//	boolean upPressed = upSwitchTrigger.triggered();
//
//	if ( ! downPressed && ! upPressed)
//	{
		readEepromHeader();
		//dumpEEPROMHeader();
		loadPatch(patchNumber);
//	}
//	else
//	{
//		Serial.println("NOT reading from EEPROM!!!!");
//	}
}

void Patch::loop(const unsigned long usNow)
{
//	downSwitchTrigger.loop(usNow);
//	boolean downPressed = downSwitchTrigger.triggered();
//	upSwitchTrigger.loop(usNow);
//	boolean upPressed = upSwitchTrigger.triggered();
	
	downSwitchEdgeProvider.loop(usNow);
	boolean downPressed = downSwitchEdgeProvider.getEdge() == OnOffEdgeProvider::OFF_ON;
	upSwitchEdgeProvider.loop(usNow);
	boolean upPressed = upSwitchEdgeProvider.getEdge() == OnOffEdgeProvider::OFF_ON;
	
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
				Serial.print(MIDI::instance().getMessageCount(false), DEC);
				Serial.println(" msgs");
				
				uint8_t nextPatchNumber = getNextPatchNumber(patchNumber, false);
				if (nextPatchNumber != patchNumber)
				{
					pendingPatchNumber = nextPatchNumber;
					
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
			else if (upPressed)
			{
				Serial.println("--> up");
				Serial.print(MIDI::instance().getMessageCount(false), DEC);
				Serial.println(" msgs");
				
				dumpEEPROMHeader();

				uint8_t nextPatchNumber = getNextPatchNumber(patchNumber, true);
				if (nextPatchNumber != patchNumber)
				{
					pendingPatchNumber = nextPatchNumber;
					
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

uint8_t Patch::incPatchNumber(const uint8_t n, const boolean up)
{
	uint8_t newN = n;
	
	if (up)
	{
		newN++;
		if (newN == MAX_PATCHES)
			newN = 0;
	}
	else
	{
		if (newN == 0)
			newN = MAX_PATCHES - 1;
		else
			newN--;
	}
	
	return newN;
}

uint8_t Patch::getNextFreePatchNumber(const uint8_t currentPatchNumber)
{
	uint8_t next = currentPatchNumber;
	do
	{
		next = incPatchNumber(next, true);
		
		if (0)
		{
			Serial.print("Patch::getNextFreePatchNumber next ");
			Serial.println(next, DEC);
		}
		
		if (next == currentPatchNumber) // wrapped
		{
			// up to caller to interpret the same patch number being returned
			return currentPatchNumber;
		}
		
	} while (eepromHeader.patchInfo[next].numCCMessages != 0);
	
	return next;
}

/**
 * Finds and returns the number of the next patch that has a valid start address
 * in EEPROM.
 */
uint8_t Patch::getNextPatchNumber(const uint8_t currentPatchNumber, const boolean searchUp)
{
	//uint8_t next = searchUp ? currentPatchNumber + 1 : currentPatchNumber - 1;
	uint8_t next = incPatchNumber(currentPatchNumber, searchUp);
	
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
//		if (searchUp)
//		{
//			next++;
//			if (next == MAX_PATCHES)
//				next = 0;
//		}
//		else
//		{
//			if (next == 0)
//				next = MAX_PATCHES - 1;
//			else
//				next--;
//		}
		
		next = incPatchNumber(next, searchUp);
		
		if (next == currentPatchNumber) // wrapped
			break;
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
	
	unsigned int nRead = EEPROM::instance().read(HEADER_START_ADDR, buffer, sizeof(buffer));
	
	if (nRead != sizeof(buffer))
	{
		Display::instance().displayError(PATCH_READ_HEADER_FROM_EEPROM_FAILED);
		return;
	}
	
	//dumpBuffer(buffer, nRead);
	
	//eepromHeader.displayBrightness = 0x7f;
	//eepromHeader.selectedPatch = 0;
	
	uint16_t index = 0;
	
	uint16_t magicNumber = *((uint16_t*)&buffer[index]);
	if (magicNumber == MAGIC_NUMBER) // OK to read the header
	{
		eepromHeader = *((EEPROMHeader*)buffer);
	}
	
	/*
	index += sizeof(magicNumber);
	if (magicNumber == MAGIC_NUMBER) // OK to read the header
	{
		eepromHeader.checksum = *((uint16_t*)&buffer[index]);
		index += sizeof(eepromHeader.checksum);
		eepromHeader.displayBrightness = buffer[index];
		index += sizeof(eepromHeader.displayBrightness);
//		eepromHeader.selectedPatch = buffer[index];
//		index += sizeof(selectedPatch);
		
		uint8_t* pBuffer = &buffer[index];
		
		for (uint8_t i=0; i<MAX_PATCHES; i++)
		{
			eepromHeader.patchInfo[i].numCCMessages = *pBuffer;
			pBuffer++;
			eepromHeader.patchInfo[i].checksum = *((uint16_t*)pBuffer);
			pBuffer += 2;
		}
	}
*/
	// now got all header info
	Display::instance().setBrightness(eepromHeader.displayBrightness);
}

// read the given patch details from EEPROM and apply settings
// return true for OK
boolean Patch::loadPatch(const uint8_t n)
{
	boolean ok = true;
	
	if (1)
	{
		Serial.print("Patch::loadPatch ");
		Serial.print(n, HEX);
		Serial.println();
	}
	
	if (eepromHeader.magicNumber == MAGIC_NUMBER)
	{
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
		
		uint16_t checksum = calcChecksum(buffer, lengthInBytes);
		if (checksum != eepromHeader.patchInfo[n].checksum)
		{
			Display::instance().displayError(PATCH_CHECKSUM_ERROR);
			ok = false;
		}
		else
		{
			MIDI::instance().processBuffer(buffer, lengthInBytes, true);
		}
	}
	else // get the default MIDI messages and transmit
	{
		uint8_t buffer[MAX_PATCH_SIZE];
		unsigned int length = 0;
		
		boolean gotAll = MIDI::instance().getListenerSettingMessages(buffer, MAX_PATCH_SIZE, length);
		MIDI::instance().processBuffer(buffer, length, true);
	}
		
	if (ok)
		Display::instance().setPatchNumber(n);
	
	return ok;
}

/**
 * Saves the header to EEPROM if the checksum has changed.
 */
void Patch::saveHeader()
{
	Serial.println("Patch::saveHeader");

	uint16_t headerChecksum = calcHeaderChecksum();
	
	if (headerChecksum != eepromHeader.checksum) // need to save
	{
		eepromHeader.magicNumber = MAGIC_NUMBER;
		eepromHeader.checksum = headerChecksum;
		
		if (1)
		{
			Serial.print("Patch::saveHeader header checksum ");
			Serial.print(eepromHeader.checksum, HEX);
			Serial.print(" to addr 0x");
			Serial.print(HEADER_START_ADDR, HEX);
			Serial.print(" from addr 0x");
			Serial.print((unsigned long)((uint8_t*)&eepromHeader), HEX);
			Serial.print(" length ");
			Serial.println(sizeof(eepromHeader), DEC);
		}
		
		uint16_t nWritten = EEPROM::instance().write(HEADER_START_ADDR, (uint8_t*)&eepromHeader, sizeof(eepromHeader));
		if (nWritten != sizeof(eepromHeader))
		{
			Serial.print(" EEPROM write failed, length ");
			Serial.println(sizeof(eepromHeader), DEC);
			Serial.print(" nWritten ");
			Serial.println(nWritten, DEC);
			
			Display::instance().displayError(HEADER_WRITE_TO_EEPROM_FAILED);
			return;
		}
	}
	else
	{
		Serial.println("Patch::saveHeader - checksum hasn't changed");
	}
}

void Patch::save()
{
	Serial.println("Patch::save");
	
	//dumpEEPROMHeader();

	uint8_t buffer[MAX_PATCH_SIZE];
	unsigned int length = 0;
	
	boolean gotAll = MIDI::instance().getListenerSettingMessages(buffer, MAX_PATCH_SIZE, length);
	if ( ! gotAll)
	{
		Display::instance().displayError(PATCH_SAVE_BUFFER_TOO_SMALL);
		return;
	}
				
	uint16_t address = PATCH_START_ADDR + patchNumber * MAX_PATCH_SIZE;
		
	uint16_t nWritten = EEPROM::instance().write(address, buffer, length);
	if (nWritten != length)
	{
		Display::instance().displayError(PATCH_WRITE_TO_EEPROM_FAILED);
		return;
	}
	
	MIDI::instance().resetMessageCount();
	eepromHeader.patchInfo[patchNumber].numCCMessages = length / 3;
	eepromHeader.patchInfo[patchNumber].checksum = calcChecksum(buffer, length);
	
	saveHeader();
	
	//dumpBuffer(buffer, length);
	dumpEEPROMHeader();
	
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
	Serial.println("Patch::copy()");
	
	uint8_t newPatchNumber = getNextFreePatchNumber(patchNumber);
	
	if (1)
	{
		Serial.print("  newPatchNumber ");
		Serial.println(newPatchNumber, DEC);
	}
	
	if (newPatchNumber == patchNumber)
	{
		Display::instance().displayError(NO_FREE_PATCHES);
		return;
	}
	
	// copy over header info
	eepromHeader.patchInfo[newPatchNumber] = eepromHeader.patchInfo[patchNumber];
	patchNumber = newPatchNumber;
	Display::instance().setPatchNumber(patchNumber);
	MIDI::instance().resetMessageCount();
	
	// transmit to iPad?
	// need to make dirty
}

void Patch::dumpEEPROMHeader()
{
	Serial.println("EEPROM header:");
	Serial.print("magic: ");
	Serial.print(eepromHeader.magicNumber, HEX);
	Serial.print(", checksum: ");
	Serial.print(eepromHeader.checksum, HEX);
	Serial.print(", displayBrightness: ");
	Serial.print(eepromHeader.displayBrightness, HEX);
//	Serial.print(", selectedPatch: ");
//	Serial.print(eepromHeader.selectedPatch, HEX);
	Serial.println();
	
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

void Patch::initHeader()
{
	eepromHeader.magicNumber = 0;
	eepromHeader.checksum = 0;
	eepromHeader.displayBrightness = 0x7f;
	for (unsigned int i=0; i<MAX_PATCHES; i++)
	{
		eepromHeader.patchInfo[i].checksum = 0;
		eepromHeader.patchInfo[i].numCCMessages = 0;
	}
}

void Patch::erase(const uint8_t value)
{
	if (value == ERASE_ALL_VALUE) // erase all
	{
		Serial.println("Patch::erase ALL");
		
		uint8_t zero[] = {0, 0};
		EEPROM::instance().write(HEADER_START_ADDR, zero, sizeof(zero));
		
		initHeader();
		patchNumber = 0;
		loadPatch(patchNumber);
	}
	else if (value == ERASE_CURRENT_VALUE) // erase current patch
	{
		Serial.println("Patch::erase CURRENT");
		
		PatchInfo empty;
		eepromHeader.patchInfo[patchNumber] = empty;
		patchNumber = getNextPatchNumber(patchNumber, false);
		//eepromHeader.checksum = calcHeaderChecksum();
		saveHeader();
		MIDI::instance().resetMessageCount();
		loadPatch(patchNumber);
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

// simple checksum for now
uint16_t Patch::calcChecksum(const uint8_t* buffer, const unsigned int length)
{
	uint16_t checksum = 0;
	for (unsigned int i=0; i<length; i++)
	{
		checksum += buffer[i];
	}
	return checksum;
}

uint16_t Patch::calcHeaderChecksum()
{
	return calcChecksum((uint8_t*)&(eepromHeader.displayBrightness), sizeof(eepromHeader) - sizeof(eepromHeader.checksum));
}
