#ifndef __CV2__PitchFork__
#define __CV2__PitchFork__

#include <Arduino.h>
#include "MIDICCListener.h"
#include "MIDIMessages.h"
#include "OnOffTriggerable.h"

class PitchFork : public OnOffTriggerable
{
public:
	PitchFork(const uint8_t aMidiChannel);
	virtual ~PitchFork();
	void setup();
	void loop(const unsigned long usNow);
	virtual void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value);
	
	/**
	 * Returns the current value of the given controller number. If the MSB is
	 * set in the return value, it should not be saved (TODO: review whether
	 * this should be controlled separately).
	 */
	virtual uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	void txCCMessage(const uint8_t cc, const uint8_t value) const;
	
	uint8_t shift;
	boolean latchEnabled;
	typedef enum {UP, DOWN, BOTH } Octave;
	Octave octave;
	uint8_t blend;
	uint8_t midiChannel;
	boolean triggeredState;
};

#endif // __CV2__PitchFork__