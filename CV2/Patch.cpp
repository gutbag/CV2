#include "Patch.h"
#include "Switch.h"
#include "Display.h"
#include "MIDI.h"
#include "EEPROM.h"

Patch::Patch(Switch& aDownSwitch, Switch& anUpSwitch)
: downSwitchTrigger(aDownSwitch), upSwitchTrigger(anUpSwitch), patchNumber(0)
{
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

void Patch::setup()
{
	downSwitchTrigger.setup();
	upSwitchTrigger.setup();
	
	MIDI::instance().setPCListener(this);
	
	Display::instance().setPatchNumber(patchNumber);
}

void Patch::loop(const unsigned long usNow)
{
	downSwitchTrigger.loop(usNow);
	upSwitchTrigger.loop(usNow);
	
	if (downSwitchTrigger.triggered())
	{
		Display::instance().set("  dn");
	}
	else if (upSwitchTrigger.triggered())
	{
		Display::instance().set("  up");
	}
}

void Patch::readEepromHeader()
{
	uint8_t buffer[128];
	
	unsigned int nRead = EEPROM::instance().read(0x0, buffer, sizeof(buffer));
	
	if (nRead != 128)
	{
		// TODO: what? display error and halt?
	}
	
	uint16_t readChecksum = *((uint16_t*)&buffer[0]);
	uint16_t length = *((uint16_t*)&buffer[2]);
	uint8_t selectedPatch = buffer[4];
	uint8_t displayBrightness = buffer[5];
	
	uint8_t* pBuffer = &buffer[6];
	uint16_t bytesConsumed = 0;
	
	while (bytesConsumed < length)
	{
		uint8_t patchNumber = *pBuffer;
		pBuffer++;
		patchInfo[patchNumber].address = *((uint16_t*)pBuffer);
		pBuffer += 2;
	}
	
	// TODO: handle length < 128 (read EEPROM again)
	
	// now got all header info
	Display::instance().setBrightness(displayBrightness);
	loadPatch(selectedPatch);
}

// read the given patch details from EEPROM and apply settings
void Patch::loadPatch(const uint8_t n)
{
	if (patchInfo[n].address == 0)
	{
		// TODO: error - do what?
	}

	uint8_t buffer[128];
	
	unsigned int nRead = EEPROM::instance().read(patchInfo[n].address, buffer, sizeof(buffer));
	
	if (nRead != 128)
	{
		// TODO: what? display error and halt?
	}
	
	uint16_t readChecksum = *((uint16_t*)&buffer[0]);
	uint16_t length = *((uint16_t*)&buffer[2]);
	
	
}
