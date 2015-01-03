#ifndef __CV2__Tempo__
#define __CV2__Tempo__

#include <Arduino.h>
#include "MIDICCListener.h"
#include "Flasher.h"
#include "OnOffTriggerable.h"
#include "OnOffEdgeProvider.h"

class Tempo : public OnOffTriggerable // , MIDICCListener,
{
public:
	Tempo();
	virtual ~Tempo();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	static const uint8_t MIN_BPM = 59; // actually one below the min - this value disabled tempo display
	static const unsigned int BEAT_DISPLAY_INDEX = 0;
	static const char BEAT_CHARS[2];
	
	static const unsigned int SHIFT_REG_SIZE = 6;
	static const unsigned int BPM_CALC_NUM_TIMES_MIN = 3;
	
	void deleteFirstTapTime();
	void updateBpm();
	void processTap(const unsigned long usTime);
	
	OnOffEdgeProvider edgeProvider;
	
	unsigned long tapTimes[SHIFT_REG_SIZE];
	unsigned int nextTapTimeIndex;
	
	float bpm; // need fractional BPM for tap tempo
	Flasher flasher;
	unsigned long lastBeatUs;
	unsigned long usBetweenBeats;
	uint8_t beatCharIndex;
};

#endif /* defined(__CV2__Tempo__) */
