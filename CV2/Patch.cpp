#include "Patch.h"
#include "Switch.h"
#include "Display.h"

Patch::Patch(Switch& aDownSwitch, Switch& anUpSwitch)
: downSwitchTrigger(aDownSwitch), upSwitchTrigger(anUpSwitch), patchNumber(0)
{
	
}

Patch::~Patch()
{
	
}

void Patch::processPCMessage(const uint8_t channel, const uint8_t programNumber)
{
	
}

void Patch::setup()
{
	downSwitchTrigger.setup();
	upSwitchTrigger.setup();
	
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
