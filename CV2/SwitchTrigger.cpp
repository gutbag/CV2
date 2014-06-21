#include "SwitchTrigger.h"
#include "Switch.h"

SwitchTrigger::SwitchTrigger(Switch& aSwitch)
: theSwitch(aSwitch), triggeredState(false), switchOn(false), triggerOffToOnState(false)
{
	
}

SwitchTrigger::~SwitchTrigger()
{
	
}

void SwitchTrigger::setup()
{
	theSwitch.setup();
	switchOn = theSwitch.isOn();
}

void SwitchTrigger::loop(const unsigned long usNow)
{
	theSwitch.loop(usNow);
	boolean newSwitchOn = theSwitch.isOn();
	
	if (triggerOffToOnState)
	{
		if ( ! switchOn && newSwitchOn)
			triggeredState = true;
		else
			triggeredState = false;
	}
	else
	{
		if (switchOn &&  ! newSwitchOn)
			triggeredState = true;
		else
			triggeredState = false;
	}
	
	switchOn = newSwitchOn;
}

// returns triggered state and clears it
boolean SwitchTrigger::triggered()
{
	boolean returnState = triggeredState;
	triggeredState = false;
	return returnState;
}

//void SwitchTrigger::triggerOnToOff(const boolean state)
//{
//	triggerOffToOnState = ! state;
//}
