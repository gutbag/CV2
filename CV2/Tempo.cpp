#include "Tempo.h"
#include "MIDI.h"
#include "Display.h"

const char Tempo::BEAT_CHARS[2] = {'^', 'u'};

Tempo::Tempo()
: bpm(MIN_BPM),
  flasher(100 * 1000, 200 * 1000),
  lastBeatUs(0),
  usBetweenBeats(0),
  beatCharIndex(0)
{
}

Tempo::~Tempo()
{
}

void Tempo::setup()
{
	MIDI::instance().setCCListener(this, 0, TEMPO_BPM_CC);
}

void Tempo::loop(const unsigned long usNow)
{
	if (bpm > MIN_BPM)
	{
		if (usNow - lastBeatUs > usBetweenBeats)
		{
			beatCharIndex++;
			if (beatCharIndex >= sizeof(BEAT_CHARS))
				beatCharIndex = 0;
//			flasher.flash();
			Display::instance().setDigit(DECIMAL_POINT_INDEX, BEAT_CHARS[beatCharIndex]);
			lastBeatUs += usBetweenBeats;
		}
		
//		flasher.loop(usNow);
		//Display::instance().setDecimalPoint(DECIMAL_POINT_INDEX, flasher.getState());
//		Display::instance().setDigit(DECIMAL_POINT_INDEX, flasher.getState() ? BEAT_CHARS[beatCharIndex] : ' ');
	}
	else
		Display::instance().setDigit(DECIMAL_POINT_INDEX, ' ');
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
			break;
		}
		default:
			break;
	}
}

uint8_t Tempo::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case TEMPO_BPM_CC:
			return bpm - MIN_BPM;
		default:
			0;
	}
}
