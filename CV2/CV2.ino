#include "Arduino.h"
#include "Display.h"
#include "DAC.h"
#include "Footswitch.h"
#include "Opto.h"
#include "MIDI.h"

#define OPTO_1 24
#define OPTO_2 25
#define OPTO_3 22
#define OPTO_4 15
#define OPTO_5 16
#define OPTO_6 17

#define FSW_1 46
#define FSW_2 47
#define FSW_3 48
#define FSW_LED1 36
#define FSW_LED2 34
#define FSW_LED3 32

#define PATCH_DOWN_SW 52
#define PATCH_UP_SW 53

#define D_SPARE_1 49
#define D_SPARE_2 50
#define D_SPARE_3 51

#define CS_PIN_5V_DAC 10
#define CS_PIN_9V_DAC 4

Display display;
DAC dac5V(CS_PIN_5V_DAC);
DAC dac9V(CS_PIN_9V_DAC);
Footswitch fsw1(FSW_1, FSW_LED1);
Footswitch fsw2(FSW_2, FSW_LED2);
Footswitch fsw3(FSW_3, FSW_LED3);
Opto optos[6] = {OPTO_1, OPTO_2, OPTO_3, OPTO_4, OPTO_5, OPTO_6};
MIDI midi;

void setup()
{
	Serial.begin(115200);
	
	display.setup();
	dac5V.setup();
	dac9V.setup();
	fsw1.setup();
	fsw2.setup();
	fsw3.setup();
	midi.setup();
	
	for (uint8_t i=0; i<6; i++)
		optos[i].setup();
	
	pinMode(PATCH_DOWN_SW, INPUT_PULLUP);
	pinMode(PATCH_UP_SW, INPUT_PULLUP);
	
	pinMode(D_SPARE_1, INPUT_PULLUP);
	pinMode(D_SPARE_2, INPUT_PULLUP);
	pinMode(D_SPARE_3, INPUT_PULLUP);
}

char c[] = {'?', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'C', 'd', 'e', 'f', 'E', 'r', 'r', ' ', '-', '_'};
int i = 0;
unsigned long t = 0;

void loop()
{
    long usNow = micros();
    display.loop(usNow);
	fsw1.loop(usNow);
	fsw2.loop(usNow);
	fsw3.loop(usNow);
	midi.loop(usNow);
    
    if (usNow - t > 600000)
    {
        i++;
        if (i > sizeof(c)-4)
            i=0;
        
        display.set(&c[i]);
        t = usNow;
    }
    
}
