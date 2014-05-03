#include "DisplayTest.h"
#include "Display.h"

static char c[] = {'?', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'C', 'd', 'e', 'f', 'E', 'r', 'r', ' ', '-', '_'};


DisplayTest::DisplayTest()
: lastUpdateUs(0), charIndex(0)
{
}

DisplayTest::~DisplayTest()
{
}

void DisplayTest::setup()
{
	Display::instance().set("    ");
}

void DisplayTest::loop(unsigned long usNow)
{
	if (usNow - lastUpdateUs > 300000)
    {
        charIndex++;
        if (charIndex > sizeof(c)-4)
            charIndex=0;
        
        Display::instance().set(&c[charIndex]);
        lastUpdateUs = usNow;
    }
}
