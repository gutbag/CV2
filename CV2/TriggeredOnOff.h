#ifndef __CV2__TriggeredOnOff__
#define __CV2__TriggeredOnOff__

#include <Arduino.h>
#include "OnOffStateProvider.h"
#include "OnOffEdgeProvider.h"

class OnOffStateProvider;

class TriggeredOnOff : public OnOffStateProvider
{
public:
	typedef enum {LEVEL_NON_INV, LEVEL_INV, EDGE_OFF_ON, EDGE_ON_OFF} Mode;
	TriggeredOnOff();
	virtual ~TriggeredOnOff();
	void setup();
	void loop(const unsigned long usNow);
	void setProvider(OnOffStateProvider* pOnOffStateProvider);
	void setOn(const boolean state);
	boolean isOn();
	void setMode(const TriggeredOnOff::Mode aMode);
private:
	OnOffStateProvider* pProvider;
	OnOffEdgeProvider edgeProvider;
	Mode mode;
	boolean on;
};

#endif /* defined(__CV2__TriggeredOnOff__) */
