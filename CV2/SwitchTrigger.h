#ifndef __CV2__SwitchTrigger__
#define __CV2__SwitchTrigger__

#include <Arduino.h>
#include "Trigger.h"

class Switch;

class SwitchTrigger : public Trigger
{
public:
	SwitchTrigger(Switch& aSwitch);
	virtual ~SwitchTrigger();
	void triggerOnToOff(const boolean state);
	void setup();
	void loop(const unsigned long usNow);
	boolean triggered();
private:
	Switch& theSwitch;
	boolean triggeredState;
	boolean switchOn;
	boolean triggerOffToOnState;
};

#endif /* defined(__CV2__SwitchTrigger__) */
