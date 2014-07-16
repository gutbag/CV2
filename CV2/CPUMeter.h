#ifndef __CV2__CPUMeter__
#define __CV2__CPUMeter__

#include <Arduino.h>
#include "MIDICCListener.h"

class CPUMeter : public MIDICCListener
{
public:
	CPUMeter();
	virtual ~CPUMeter();
	void setup();
	void loop(const unsigned long usNow);
	void processCCMessage(const uint8_t channel,
						  const uint8_t controllerNumber,
						  const uint8_t value);
	uint8_t getControllerValue(const uint8_t controllerNumber);
private:
	static const unsigned long MEAS_PERIOD_US = 1000000; // 1s
//	static const unsigned long MAX_COUNTS = 220000; // TODO: the number of counts for 0% utilisation
//	static const unsigned long COUNTS_PER_PERCENT = MAX_COUNTS / 100;
	typedef enum {NO_MEAS, MIN_LOOP_TIME, MAX_LOOP_TIME, AVG_LOOP_TIME} MeasType;
	MeasType controlValue;
	unsigned long count;
	unsigned long usLastMeasPeriodStart;
//	unsigned long history[10];
//	uint8_t historyIndex;
	unsigned long lastLoopTime;
	unsigned long loopTimeSum;
	unsigned long minLoopTime;
	unsigned long maxLoopTime;
};

#endif /* defined(__CV2__CPUMeter__) */
