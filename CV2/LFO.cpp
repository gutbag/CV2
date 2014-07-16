#include "LFO.h"
#include "MIDI.h"

static uint8_t sinSamples[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	1,
	1,
	1,
	2,
	2,
	2,
	3,
	3,
	4,
	4,
	5,
	6,
	6,
	7,
	8,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	22,
	23,
	24,
	26,
	27,
	28,
	30,
	31,
	33,
	34,
	36,
	37,
	39,
	40,
	42,
	44,
	45,
	47,
	49,
	51,
	52,
	54,
	56,
	58,
	60,
	62,
	63,
	65,
	67,
	69,
	71,
	73,
	75,
	77,
	79,
	81,
	84,
	86,
	88,
	90,
	92,
	94,
	96,
	98,
	101,
	103,
	105,
	107,
	109,
	112,
	114,
	116,
	118,
	120,
	123,
	125,
	127,
	127,
	129,
	131,
	134,
	136,
	138,
	140,
	142,
	145,
	147,
	149,
	151,
	153,
	156,
	158,
	160,
	162,
	164,
	166,
	168,
	170,
	173,
	175,
	177,
	179,
	181,
	183,
	185,
	187,
	189,
	191,
	192,
	194,
	196,
	198,
	200,
	202,
	203,
	205,
	207,
	209,
	210,
	212,
	214,
	215,
	217,
	218,
	220,
	221,
	223,
	224,
	226,
	227,
	228,
	230,
	231,
	232,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	242,
	243,
	244,
	245,
	246,
	246,
	247,
	248,
	248,
	249,
	250,
	250,
	251,
	251,
	252,
	252,
	252,
	253,
	253,
	253,
	254,
	254,
	254,
	254,
	254,
	254,
	254,
	254,
	254,
	254,
	254,
	253,
	253,
	253,
	252,
	252,
	252,
	251,
	251,
	250,
	250,
	249,
	248,
	248,
	247,
	246,
	246,
	245,
	244,
	243,
	242,
	241,
	240,
	239,
	238,
	237,
	236,
	235,
	234,
	232,
	231,
	230,
	228,
	227,
	226,
	224,
	223,
	221,
	220,
	218,
	217,
	215,
	214,
	212,
	210,
	209,
	207,
	205,
	203,
	202,
	200,
	198,
	196,
	194,
	192,
	191,
	189,
	187,
	185,
	183,
	181,
	179,
	177,
	175,
	173,
	170,
	168,
	166,
	164,
	162,
	160,
	158,
	156,
	153,
	151,
	149,
	147,
	145,
	142,
	140,
	138,
	136,
	134,
	131,
	129,
	127,
	125,
	123,
	120,
	118,
	116,
	114,
	112,
	109,
	107,
	105,
	103,
	101,
	98,
	96,
	94,
	92,
	90,
	88,
	86,
	84,
	81,
	79,
	77,
	75,
	73,
	71,
	69,
	67,
	65,
	63,
	62,
	60,
	58,
	56,
	54,
	52,
	51,
	49,
	47,
	45,
	44,
	42,
	40,
	39,
	37,
	36,
	34,
	33,
	31,
	30,
	28,
	27,
	26,
	24,
	23,
	22,
	20,
	19,
	18,
	17,
	16,
	15,
	14,
	13,
	12,
	11,
	10,
	9,
	8,
	8,
	7,
	6,
	6,
	5,
	4,
	4,
	3,
	3,
	2,
	2,
	2,
	1,
	1,
	1,
	0,
	0,
	0,
	0,
	0
};

typedef struct
{
	float fMin;
	float fMax;
} RangeValues;

// ranges selected by CC value
static const RangeValues rangeValues[] =
{
	{0.01, 0.05},
	{0.05, 0.1},
	{0.1, 1.0},
	{1.0, 5.0},
	{0.1, 5.0}
};

static LFO* pInstances[2] = {NULL, NULL};

LFO& LFO::instance(const uint8_t index)
{
	return *pInstances[index];
}

LFO::LFO(const uint8_t index, const uint8_t aMidiChannel)
: OnOffTriggerable(aMidiChannel, LFO_TRIGGER_INSTANCE_CC),
  sampleIndex(0), usLastSample(0), usBetweenSamples(0),
  freqStep(aMidiChannel, LFO_FREQUENCY_MIN_CC, LFO_FREQUENCY_MAX_CC, LFO_FREQUENCY_SOURCE_CC),
  freqRange(0), freqStepValue(0),
  midiChannel(aMidiChannel)
{
	pInstances[index] = this;
	setDefaults(true, true);
}

LFO::~LFO()
{
}

uint16_t LFO::getMinimum()
{
	return 0;
}

uint16_t LFO::getMaximum()
{
	return 255;
}

uint16_t LFO::getValue()
{
	return sinSamples[sampleIndex];
}

void LFO::setup()
{
	OnOffTriggerable::setup();
	freqStep.setup();
	MIDI::instance().setCCListener(this, midiChannel, LFO_FREQUENCY_RANGE_CC);
	
	freqStep.setMinimum(0);
	freqStep.setMaximum(127);

	setFrequencyRange(freqRange);
	setFrequency(true);
}

void LFO::loop(const unsigned long usNow)
{
	if (usNow - usLastSample >= usBetweenSamples)
	{
		boolean triggered = isTriggered();
		
		if (triggered)
		{
			sampleIndex++;
			if (sampleIndex > sizeof(sinSamples)) // TODO: sample width?
				sampleIndex = 0;
			
			usLastSample = usNow;
			
			setFrequency(false);
		}
	}
}

void LFO::setFrequency(const boolean force)
{
	uint16_t freqStepNewValue = freqStep.getValue();
	
	if (force || (freqStepNewValue != freqStepValue)) // re-calc delay between samples
	{
		freqStepValue = freqStepNewValue;
	
		float f;
		
		if (freqStepValue == 0)
			f = fMin;
		else
			f = fMin + hertzPerStep * (float)freqStepValue;
		
		float period = 1.0 / f;
		usBetweenSamples = (unsigned long)(1.0E6 * period / (float)(sizeof(sinSamples)) + 0.5);
	}
	
	//	Serial.print("LFO setFrequency: freqStep: ");
	//	Serial.print(freqStep);
	//	Serial.print(" hertzPerStep: ");
	//	Serial.print(hertzPerStep, 4);
	//	Serial.print(" (float)freqStep: ");
	//	Serial.print((float)freqStep, 4);
	//	Serial.print(" f: ");
	//	Serial.print(f, 4);
	//	Serial.print(" usBetweenSamples: ");
	//	Serial.println(usBetweenSamples, DEC);
	
	//	usBetweenSamples = 27700;
}

void LFO::setFrequencyRange(const uint8_t value)
{
	freqRange = value;
	
	uint8_t numRanges = sizeof(rangeValues) / sizeof(RangeValues);
	
	if (freqRange >= numRanges) // limit
		freqRange = numRanges - 1;
	
	fMin = rangeValues[freqRange].fMin;
	fMax = rangeValues[freqRange].fMax;
	
	hertzPerStep = (fMax - fMin) / 128.0;
}

void LFO::processCCMessage(const uint8_t channel,
							   const uint8_t controllerNumber,
							   const uint8_t value)
{
	//	Serial.print("LFO Message, ch: ");
	//	Serial.print(channel, DEC);
	//	Serial.print(" No: ");
	//	Serial.print(controllerNumber, DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(pvalue, DEC);
	
	switch (controllerNumber)
	{
		case LFO_FREQUENCY_RANGE_CC:
			setFrequencyRange(value);
			setFrequency(true);
			break;
		default:
			OnOffTriggerable::processCCMessage(channel, controllerNumber, value);
			//freqStep.processCCMessage(channel, controllerNumber, value);
			break;
	}
}

uint8_t LFO::getControllerValue(const uint8_t controllerNumber)
{
	switch (controllerNumber)
	{
		case LFO_FREQUENCY_RANGE_CC:
			return freqRange;
		default:
			return OnOffTriggerable::getControllerValue(controllerNumber);
	}
}
