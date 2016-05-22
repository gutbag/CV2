#include "CPUMeter.h"
#include "Display.h"
#include "MIDI.h"

CPUMeter::CPUMeter() : controlValue(NO_MEAS), count(0), usLastMeasPeriodStart(0) //, historyIndex(0)
{
//	for (uint8_t i=0; i<10; i++)
//		history[i] = 0;
	lastLoopTime = 0;
	loopTimeSum = 0;
	minLoopTime = ~0;
	maxLoopTime = 0;
}

CPUMeter::~CPUMeter()
{
}

void CPUMeter::setup()
{
	MIDI::instance().setCCListener(this, 0, CPU_METER_CC);
}

void CPUMeter::loop(const unsigned long usNow)
{
	if (controlValue != NO_MEAS)
	{
		count++;
		unsigned long usSinceLastLoop = usNow - lastLoopTime;
		lastLoopTime = usNow;
		
		if (usSinceLastLoop < minLoopTime)
			minLoopTime = usSinceLastLoop;
		if (usSinceLastLoop > maxLoopTime)
			maxLoopTime = usSinceLastLoop;
		loopTimeSum += usSinceLastLoop;
		
		if (usNow - usLastMeasPeriodStart >= MEAS_PERIOD_US) // display result
		{
			//			uint16_t percent = count / COUNTS_PER_PERCENT;
			//			Display::instance().displayNumber(percent);
			
			unsigned long avgLoopTime = loopTimeSum/count;
			
			switch (controlValue)
			{
				case MIN_LOOP_TIME:
					Display::instance().displayNumber(minLoopTime);
					break;
				case MAX_LOOP_TIME:
					Display::instance().displayNumber(maxLoopTime);
					break;
				case AVG_LOOP_TIME:
					Display::instance().displayNumber(avgLoopTime);
					break;
				default:
					break;
			}
			
			if (0)
			{
				Serial.print("Min: ");
				Serial.print(minLoopTime, DEC);
				Serial.print(" Max: ");
				Serial.print(maxLoopTime, DEC);
				Serial.print(" Avg: ");
				Serial.print(avgLoopTime, DEC);
				Serial.print(" usSinceLastLoop: ");
				Serial.println(usSinceLastLoop, DEC);
			}
			
			usLastMeasPeriodStart = usNow;
			count = 0;
			loopTimeSum = 0;
			minLoopTime = ~0;
			maxLoopTime = 0;
		}
	}
}

//void CPUMeter::loop(const unsigned long usNow)
//{
//	if (enabled)
//	{
//		if (usNow - usLastMeasPeriodStart >= MEAS_PERIOD_US) // display result
//		{
//			//			uint16_t percent = count / COUNTS_PER_PERCENT;
//			//			Display::instance().displayNumber(percent);
//			
//			Serial.println(count, DEC);
//			count = 0;
//			
//			if (0)
//			{
//				history[historyIndex] = count;
//				
//				if (historyIndex < 9)
//				{
//					historyIndex++;
//				}
//				else
//				{
//					for (uint8_t i=0; i<10; i++)
//					{
//						Serial.print("[");
//						Serial.print(i, DEC);
//						Serial.print("]=");
//						Serial.println(history[i], DEC);
//						history[i] = 0;
//					}
//					historyIndex = 0;
//				}
//				count = 0;
//			}
//			
//			usLastMeasPeriodStart = usNow;
//		}
//		
//		count++;
//	}
//}

//void CPUMeter::loop(const unsigned long usNow)
//{
//	if (enabled)
//	{
//		if (usNow - usLastMeasPeriodStart >= MEAS_PERIOD_US) // display result
//		{
//			uint16_t percent = count / COUNTS_PER_PERCENT;
//			Display::instance().displayNumber(percent);
//			
//			count = MAX_COUNTS; // start again
//			usLastMeasPeriodStart = usNow;			
//		}
//		
//		if (count > 0)
//		{
//			count--;
//		}
//		else // TODO: implies that the MAX is wrong, display error when the period is up?
//		{
//			//Serial.println
//		}
//	}
//}

void CPUMeter::processCCMessage(const uint8_t channel,
					  const uint8_t controllerNumber,
					  const uint8_t value)
{
	switch (controllerNumber)
	{
		case CPU_METER_CC:
			controlValue = (MeasType)value;
			count = 0;
			loopTimeSum = 0;
			minLoopTime = ~0;
			maxLoopTime = 0;
			usLastMeasPeriodStart = micros();
			break;
		default:
			break;
	}
}

uint8_t CPUMeter::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	// nothing is saved
	return DO_NOT_SAVE_VALUE;
}
