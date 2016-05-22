#ifndef __CV2__SourceValueProvider__
#define __CV2__SourceValueProvider__

#include <Arduino.h>
#include "MIDICCListener.h"
#include "ValueProvider.h"

/**
 * Provides a value that is either from a single MIDI CC, or a source module.
 */
class SourceValueProvider : public MIDICCListener
{
public:
	SourceValueProvider();
	SourceValueProvider(const uint8_t aMidiChannel, const uint8_t aMinCC, const uint8_t aMaxCC, const uint8_t aSourceCC);
	virtual ~SourceValueProvider();
	void setup();
	void processCCMessage(const uint8_t channel,
								  const uint8_t controllerNumber,
								  const uint8_t value);
	uint8_t getControllerValue(const uint8_t channel, const uint8_t controllerNumber);
	void setMinimum(const uint8_t value);
	void setMaximum(const uint8_t value);
	uint16_t getMinimum();
	uint16_t getMaximum();
	uint16_t getValue();
private:
	uint8_t midiChannel;
	uint8_t minCC;
	uint8_t maxCC;
	uint8_t sourceCC;
	ValueProvider* pSource;
	uint8_t minValue;
	uint8_t maxValue;
	uint8_t sourceId;
};

#endif /* defined(__CV2__SourceValueProvider__) */
