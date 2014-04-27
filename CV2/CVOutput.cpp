#include "CVOutput.h"
#include "DAC.h"

CVOutput::CVOutput(DAC& aDac, const uint8_t anOutput)
: dac(aDac), output(anOutput), value(0)
{
}

CVOutput::~CVOutput()
{
}

void CVOutput::setup()
{
	dac.setOutput(output, value);
}

void CVOutput::loop(const unsigned int usNow)
{
}
