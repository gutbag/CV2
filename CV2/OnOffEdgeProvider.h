#ifndef __CV2__OnOffEdgeProvider__
#define __CV2__OnOffEdgeProvider__

#include <Arduino.h>

class OnOffStateProvider;

class OnOffEdgeProvider
{
public:
	typedef enum {NONE, OFF_ON, ON_OFF} EdgeType;
	OnOffEdgeProvider();
	OnOffEdgeProvider(OnOffStateProvider* pAProvider = NULL);
	virtual ~OnOffEdgeProvider();
	void setup();
	void loop(const unsigned long usNow);
	void setOnOffProvider(OnOffStateProvider* pAProvider);
	OnOffEdgeProvider::EdgeType getEdge();
private:
	OnOffStateProvider* pProvider;
	EdgeType edge;
	boolean providerIsOn;
};

#endif /* defined(__CV2__OnOffEdgeProvider__) */
