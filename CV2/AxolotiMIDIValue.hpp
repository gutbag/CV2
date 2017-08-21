#ifndef __CV2__AxolotiMIDIValue__
#define __CV2__AxolotiMIDIValue__

#include <Arduino.h>
#include "OnOffTriggerable.h"

class AxolotiMIDIValue : public OnOffTriggerable
{
public:
	AxolotiMIDIValue(const uint8_t aCV2MidiChannel, const uint8_t anAxolotiMidiChannel, const uint8_t aControllerNumber);
	virtual ~AxolotiMIDIValue();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
private:
	static const uint8_t AXOLOTI_CTRL_CC = 2; // TODO: get in ctor from parent Axoloti class

	void txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const;
	void on(const boolean force = false);
	void off(const boolean force = false);
	
	uint8_t cv2MidiChannel;
	uint8_t axolotiMidiChannel;
	uint8_t controllerNumber;
	uint8_t onValue;
	uint8_t onState;
	boolean triggered;
	boolean debug;
};

#endif /* defined(__CV2__AxolotiMIDIValue__) */
