#include "SwitchTrigger.h"
#include "Switch.h"

SwitchTrigger::SwitchTrigger(Switch& aSwitch)
: theSwitch(aSwitch), triggeredState(false)
{
	
}

SwitchTrigger::~SwitchTrigger()
{
	
}

void SwitchTrigger::setup()
{
	theSwitch.setup();
}

void SwitchTrigger::loop(const unsigned long usNow)
{
	theSwitch.loop(usNow);
	
	if (theSwitch.isOn())
		triggeredState = true;
	else
		triggeredState = false;
		
}

boolean SwitchTrigger::triggered()
{
	return triggeredState;
}
