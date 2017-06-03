#ifndef __CV2__Axoloti__
#define __CV2__Axoloti__

#include <Arduino.h>
#include "MIDICCListener.h"
#include "SourceValueProvider.h"

class ValueProvider;

class Axoloti : public MIDICCListener
{
public:
	Axoloti();
	virtual ~Axoloti();
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
	
	static const unsigned long STARTUP_TX_DELAY_US = 3000000; // 3s
	static const uint8_t AXOLOTI_MIDI_CHANNEL = 1; // the channel for messages *to* the Axoloti
	static const unsigned long MIN_UPDATE_PERIOD_US = 2000; // much smaller and we can't keep up with the expr pedal
	
	void txCCMessage(const uint8_t ch, const uint8_t cc, const uint8_t value) const;
	void txPCMessage(const uint8_t ch, const uint8_t programNumber) const;
	void txAllValues() const;
	
	unsigned long startupUs;
	boolean startupTxDone;

	ValueProvider* pProvider;
	uint8_t sourceTypeId;
	SourceValueProvider lowValue;
	SourceValueProvider highValue;
	
	uint8_t patchNumber;
	uint8_t presetNumber;
	uint8_t varValue;
	
	unsigned long lastUpdateUs;
};

#endif // __CV2__Axoloti__
