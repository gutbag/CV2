#ifndef __CV2__TriggeredOnOff__
#define __CV2__TriggeredOnOff__

#include <Arduino.h>
#include "OnOffStateProvider.h"
#include "OnOffEdgeProvider.h"
#include "MIDICCListener.h"

class OnOffStateProvider;

class TriggeredOnOff : public OnOffStateProvider, public MIDICCListener
{
public:
	static TriggeredOnOff& instance(const uint8_t index);

	typedef enum {LEVEL_NON_INV, LEVEL_INV, EDGE_OFF_ON, EDGE_ON_OFF} Mode;
	TriggeredOnOff(const uint8_t midiChannel);
	virtual ~TriggeredOnOff();
	void setup();
	void loop(const unsigned long usNow);
	void setDefaultOn(const boolean state);
	boolean isOn();
	void setMode(const TriggeredOnOff::Mode aMode);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	void setProvider(OnOffStateProvider* pOnOffStateProvider);
	void setOn(const boolean state);

	OnOffStateProvider* pProvider;
	OnOffEdgeProvider edgeProvider;
	Mode mode;
	boolean on;
	uint8_t midiChannel;
	uint8_t triggerModeCCValue;
	uint8_t controlTypeCCValue;
	boolean defaultOn;
	boolean debugFlag;
};

#endif /* defined(__CV2__TriggeredOnOff__) */
