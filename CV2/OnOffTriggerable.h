#ifndef __CV2__OnOffTriggerable__
#define __CV2__OnOffTriggerable__

#include <Arduino.h>
#include "TriggeredOnOff.h"
#include "MIDICCListener.h"
#include "OnOffStateProvider.h"

class OnOffTriggerable : public MIDICCListener, public OnOffStateProvider
{
public:
	OnOffTriggerable(const uint8_t aTriggerInstanceCCValue, const uint8_t aMidiChannel);
	virtual ~OnOffTriggerable();
	virtual boolean isTriggered();
	virtual boolean isOn(); // OnOffStateProvider impl
	virtual void setup();
	virtual void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	virtual uint8_t getControllerValue(const uint8_t controllerNumber);
	void setDefaults(const boolean aNullTriggerOnState, const boolean aTriggerDefaultOn);
private:
	TriggeredOnOff* trigger;
	uint8_t midiChannel;
	uint8_t triggerInstanceCC;
	uint8_t triggerInstanceCCValue;
	boolean nullTriggerOnState;
	boolean triggerDefaultOn;
};

#endif /* defined(__CV2__OnOffTriggerable__) */
