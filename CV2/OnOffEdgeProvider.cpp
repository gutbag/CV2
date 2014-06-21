#include "OnOffEdgeProvider.h"
#include "OnOffStateProvider.h"

OnOffEdgeProvider::OnOffEdgeProvider(OnOffStateProvider* pAProvider /*=NULL*/)
: pProvider(pAProvider), edge(NONE), providerIsOn(false)
{
	
}

OnOffEdgeProvider::~OnOffEdgeProvider()
{
	
}

void OnOffEdgeProvider::setup()
{
	if (pProvider != NULL)
		providerIsOn = pProvider->isOn();
}

void OnOffEdgeProvider::loop(const unsigned long usNow)
{
	if (pProvider == NULL)
		return;
	
	boolean newProviderIsOn = pProvider->isOn();
	
	if (providerIsOn != newProviderIsOn) // state has changed
	{
		providerIsOn = newProviderIsOn;
		
		if (providerIsOn)
			edge = OFF_ON;
		else
			edge = ON_OFF;
	}
}

void OnOffEdgeProvider::setOnOffProvider(OnOffStateProvider* pAProvider)
{
	pProvider = pAProvider;
}

// edge is cleared on read
OnOffEdgeProvider::EdgeType OnOffEdgeProvider::getEdge()
{
	EdgeType returnEdge = edge;
	edge = NONE;
	return returnEdge;
}
