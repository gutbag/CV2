#include "DisplayTest.h"
#include "Display.h"
#include "MIDI.h"
#include "MIDIMessages.h"

static char c[] = {'?', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'C', 'd', 'e', 'f', 'E', 'r', 'r', ' ', '-', '_'};


DisplayTest::DisplayTest()
: lastCharUpdateUs(0), lastDpUpdateUs(0), charIndex(0), enabled(false)
{
	MIDI::instance().setCCListener(this, 0, DISPLAY_TEST_CC);
}

DisplayTest::~DisplayTest()
{
}

void DisplayTest::setup()
{
	if (enabled)
	{
		Display::instance().set("    ");
	}
}

void DisplayTest::loop(unsigned long usNow)
{
	if ( ! enabled)
		return;
	
	// cycle chars
	if (usNow - lastCharUpdateUs > 300000)
    {
        charIndex++;
        if (charIndex > sizeof(c)-4)
            charIndex=0;
        
        Display::instance().set(&c[charIndex]);
        lastCharUpdateUs = usNow;
    }
	
	// cycle DPs at a different rate
	if (usNow - lastDpUpdateUs > 100000)
	{
		static unsigned int dpIndex = 0;
		static boolean state = false;
		
		if (dpIndex == 0)
			state = !state;

		Display::instance().setDecimalPoint(dpIndex, state);
		dpIndex++;
		if (dpIndex > 3)
			dpIndex=0;
		
		lastDpUpdateUs = usNow;
	}
}

void DisplayTest::enable(boolean state)
{
	enabled = state;
}

void DisplayTest::processMessage(const char* pMessage)
{
	//	Serial.print("Display Message: ");
	//	Serial.print(pMessage[0], HEX);
	//	Serial.print(" No: ");
	//	Serial.print(pMessage[1], DEC);
	//	Serial.print(" Val: ");
	//	Serial.println(pMessage[2], DEC);
	
	if ((*pMessage & 0xf0) == 0xb0) // Control Change
	{
		switch (pMessage[1])
		{
			case DISPLAY_TEST_CC: // TODO: change back to switch?
				enable(pMessage[2] == 0 ? false : true);
				Display::instance().clear();
				break;
			default:
				break;
		}
	}
}

