#include "Patch.h"

void Patch::processMessage(const char* pMessage)
{
	switch (*pMessage & 0xf0)
	{
		case 0xb0: // Control Change
			printControlChange(pMessage);
			break;
		default:
			break;
	}
}

void Patch::printControlChange(const char* pMessage)
{
	Serial.print("Control Change, No: ");
	Serial.print(pMessage[1]);
	Serial.print(" Val: ");
	Serial.println(pMessage[2]);
}

