#ifndef __CV2__Tempo__
#define __CV2__Tempo__

#include <Arduino.h>
#include "MIDICCListener.h"
#include "Flasher.h"

class Tempo : public MIDICCListener
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
	static const unsigned int DECIMAL_POINT_INDEX = 0;
	static const char BEAT_CHARS[2];
	uint16_t bpm;
	Flasher flasher;
	unsigned long lastBeatUs;
	unsigned long usBetweenBeats;
	uint8_t beatCharIndex;
};

#endif /* defined(__CV2__Tempo__) */
