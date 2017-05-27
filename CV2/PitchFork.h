#ifndef __CV2__PitchFork__
#define __CV2__PitchFork__

#include <Arduino.h>
#include "MIDICCListener.h"
#include "MIDIMessages.h"
#include "OnOffTriggerable.h"
//#include "SoftMIDIOut.h"

class PitchFork : public OnOffTriggerable
{
public:
	PitchFork();
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
	virtual uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
private:
	
	static const unsigned long STARTUP_TX_DELAY_US = 1000000; // 1s
	
	void txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const;
	void txAllValues() const;
	
	uint8_t shift;
	boolean latch;
	typedef enum {UP, DOWN, BOTH } Octave;
	Octave octave;
	uint8_t blend;
	boolean triggeredState;
//	SoftMIDIOut midiOut;
	boolean shiftEnabled;
	boolean latchEnabled;
	boolean octaveEnabled;
	typedef enum {LOCAL, MIDI, CV} BlendControl;
	BlendControl blendControl;
	boolean footswitchEnabled;
	unsigned long startupUs;
	boolean startupTxDone;
};

#endif // __CV2__PitchFork__