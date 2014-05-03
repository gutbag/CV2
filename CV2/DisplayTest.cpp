#include "DisplayTest.h"
#include "Display.h"

static char c[] = {'?', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'C', 'd', 'e', 'f', 'E', 'r', 'r', ' ', '-', '_'};


DisplayTest::DisplayTest()
: lastCharUpdateUs(0), lastDpUpdateUs(0), charIndex(0), enabled(false)
{
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
