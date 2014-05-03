#include "Patch.h"
#include "CVOutput.h"

Patch::Patch()
: outputs(NULL)
{
	
}

Patch::~Patch()
{
	
}

void Patch::processMessage(const char* pMessage)
{
	switch (*pMessage & 0xf0)
	{
		case 0xb0: // Control Change
			printControlChange(pMessage);
			handleControlChange(pMessage);
			break;
		default:
			break;
	}
}

void Patch::printControlChange(const char* pMessage)
{
	Serial.print("Control Change, No: ");
	Serial.print(pMessage[1], DEC);
	Serial.print(" Val: ");
	Serial.println(pMessage[2], DEC);
}

void Patch::handleControlChange(const char* pMessage)
{
	uint8_t ch = (*pMessage & 0x0f) + 1;
	uint8_t ctrl = pMessage[1];
	uint8_t value = pMessage[2];
	
	if (ctrl == 3) // temp - min
		outputs[ch-1].setMinimum(value);
	else if (ctrl == 4)
		outputs[ch-1].setMaximum(value);
	else
	{
		
	}
}
