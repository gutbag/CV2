/*

 !!! TODO !!!
 
 Patches that haven't got saved trigger CC value will get the value from the 
 previously selected patch. Need to re-save all patches to fix.
 
 Could consider generic way for saved patches to handle new features being added.
 e.g. add 'patch-change' signal handling to the newly-added features?
 
 */

#include "Tempo.h"
#include "MIDI.h"
#include "Display.h"

const char Tempo::BEAT_CHARS[2] = {'^', 'u'};

Tempo::Tempo()
: OnOffTriggerable(0, TEMPO_TRIGGER_INSTANCE_CC),
  edgeProvider(this),
  nextTapTimeIndex(0),
  bpm(MIN_BPM),
  flasher(100 * 1000, 200 * 1000),
  lastBeatUs(0),
  usBetweenBeats(0),
  beatCharIndex(0)
{
	setDefaults(false, false);

	for (unsigned int i=0; i<SHIFT_REG_SIZE; i++)
	{
		tapTimes[i] = ~0;
	}
}

Tempo::~Tempo()
{
}

void Tempo::setup()
{
	OnOffTriggerable::setup();
	MIDI::instance().setCCListener(this, 0, TEMPO_BPM_CC);
	edgeProvider.setup();
}

void Tempo::loop(const unsigned long usNow)
{
	edgeProvider.loop(usNow);
	
	if (bpm > MIN_BPM)
	{
		if (usNow - lastBeatUs > usBetweenBeats)
		{
			beatCharIndex++;
			if (beatCharIndex >= sizeof(BEAT_CHARS))
				beatCharIndex = 0;
//			flasher.flash();
			Display::instance().setDigit(BEAT_DISPLAY_INDEX, BEAT_CHARS[beatCharIndex]);
			lastBeatUs += usBetweenBeats;
		}
	}
	else
		Display::instance().setDigit(BEAT_DISPLAY_INDEX, ' ');

	// purge stale tap tempo timestamps
	if (tapTimes[0] != ~0 && usNow - tapTimes[0] >= 4000000) // 2s?
	{
		deleteFirstTapTime();
		if (nextTapTimeIndex > 0)
			nextTapTimeIndex--;
	}
	
	if (edgeProvider.getEdge() == OnOffEdgeProvider::OFF_ON)
	{
		processTap(usNow);
	}
}

void Tempo::processCCMessage(const uint8_t channel,
					  const uint8_t controllerNumber,
					  const uint8_t value)
{
	switch (controllerNumber)
	{
		case TEMPO_BPM_CC:
		{
			bpm = MIN_BPM + value;
			float beatsPerSecond = ((float)bpm) / 60.0;
			usBetweenBeats = 1000000.0 / beatsPerSecond + 0.5;
			
			if (bpm > MIN_BPM)
			{
				char s[4];
				Display::instance().format(bpm, s);
				Display::instance().setAlternateDisplay(s);
			}
			else
			{
				Display::instance().setAlternateDisplay("\0\0\0\0");
			}
			
			break;
		}
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t Tempo::getControllerValue(const uint8_t channel, const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case TEMPO_BPM_CC:
			return bpm - MIN_BPM;
		default:
			return OnOffTriggerable::getControllerValue(channel, controllerNumber);
	}
}

void Tempo::deleteFirstTapTime()
{
	for (unsigned int i=0; i<SHIFT_REG_SIZE-1; i++)
	{
		tapTimes[i] = tapTimes[i+1];
	}
	tapTimes[SHIFT_REG_SIZE-1] = ~0;
}

void Tempo::updateBpm()
{
	unsigned long sum = 0;
	unsigned int i = 0;
	
	while (i<(SHIFT_REG_SIZE-1) && tapTimes[i+1] != ~0)
	{
		unsigned long diff = tapTimes[i+1] - tapTimes[i];
		sum += diff;
		i++;
	}
	
	if (i >= BPM_CALC_NUM_TIMES_MIN) // enough times
	{
		unsigned long meanMs = (sum / i) / 1000;
		
		bpm = 60.0 / ((float)meanMs / 1000.0);
		
		usBetweenBeats = 1000000.0 / bpm + 0.5;
	}
	
	// always display, even for one tap
	char s[4];
	Display::instance().format((uint16_t)(bpm+0.5), s);
	Display::instance().setAlternateDisplay(s);
}

void Tempo::processTap(const unsigned long usTime)
{
	if (nextTapTimeIndex > (SHIFT_REG_SIZE-1))
	{
		deleteFirstTapTime();
		nextTapTimeIndex = SHIFT_REG_SIZE - 1;
	}
	
	tapTimes[nextTapTimeIndex] = usTime;
	
	updateBpm();
	
	nextTapTimeIndex++;
}
