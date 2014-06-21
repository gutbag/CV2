#include "TriggeredOnOff.h"

/**
 * OnOffStateProvider with state changes triggered by either level or edge
 * changes in its own provider. Can be configured to follow the provider's level
 * or toggle when the provider's state changes. Either can be inverted.
 *
 * TODO: both edges is pointless - same as level
 */

TriggeredOnOff::TriggeredOnOff()
: pProvider(NULL), edgeProvider(NULL), mode(LEVEL_NON_INV), on(false)
{
}

TriggeredOnOff::~TriggeredOnOff()
{
}

void TriggeredOnOff::setup()
{
	edgeProvider.setup();
}

void TriggeredOnOff::loop(const unsigned long usNow)
{
	if (pProvider == NULL)
		return;
	
	if (mode == LEVEL_NON_INV)
	{
		on = pProvider->isOn();
	}
	else if (mode == LEVEL_INV)
	{
		on =  ! pProvider->isOn();
	}
	else // edge
	{
		edgeProvider.loop(usNow);
		
		OnOffEdgeProvider::EdgeType edge = edgeProvider.getEdge();
		
		if (edge != OnOffEdgeProvider::NONE) // edge detected
		{
			if (mode == EDGE_OFF_ON && edge == OnOffEdgeProvider::OFF_ON ||
				mode == EDGE_ON_OFF && edge == OnOffEdgeProvider::ON_OFF)
			{
				on = !on;
			}
		}
	}
}

void TriggeredOnOff::setProvider(OnOffStateProvider* pOnOffStateProvider)
{
	pProvider = pOnOffStateProvider;
	edgeProvider.setOnOffProvider(pProvider);
	on = false;
}

void TriggeredOnOff::setOn(const boolean state)
{
	on = state;
}

boolean TriggeredOnOff::isOn()
{
	return on;
}

void TriggeredOnOff::setMode(const TriggeredOnOff::Mode aMode)
{
	mode = aMode;
}