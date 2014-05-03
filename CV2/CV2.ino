#include "Arduino.h"
#include "Display.h"
#include "DAC.h"
#include "Footswitch.h"
#include "Opto.h"
#include "MIDI.h"
#include "CVOutput.h"
#include "LFO.h"
#include "Patch.h"
#include "DisplayTest.h"

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
MIDI midi;
DAC dac5V(CS_PIN_5V_DAC);
DAC dac9V(CS_PIN_9V_DAC);
CVOutput cvOutputs5V[8] =
{
	{dac5V, 0},
	{dac5V, 1},
	{dac5V, 2},
	{dac5V, 3},
	{dac5V, 4},
	{dac5V, 5},
	{dac5V, 6},
	{dac5V, 7}
};
CVOutput cvOutputs9V[6] =
{
	{dac9V, 0},
	{dac9V, 1},
	{dac9V, 2},
	{dac9V, 3},
	{dac9V, 4},
	{dac9V, 5}
};
Footswitch fsw1(FSW_1, FSW_LED1);
Footswitch fsw2(FSW_2, FSW_LED2);
Footswitch fsw3(FSW_3, FSW_LED3);
Opto optos[6] = {OPTO_1, OPTO_2, OPTO_3, OPTO_4, OPTO_5, OPTO_6};
LFO lfo;
Patch patch;
DisplayTest displayTest;

void setup()
{
	Serial.begin(115200);
	
	display.setup();
	midi.setup();
	dac5V.setup();
//	dac9V.setup();
	fsw1.setup();
	fsw2.setup();
	fsw3.setup();
	lfo.setup();
	
	displayTest.enable(false);
	displayTest.setup();
	
	for (uint8_t i=0; i<(sizeof(optos)/sizeof(Opto)); i++)
		optos[i].setup();
	
	for (uint8_t i=0; i<(sizeof(cvOutputs5V)/sizeof(CVOutput)); i++)
		cvOutputs5V[i].setup();
	
//	for (uint8_t i=0; i<(sizeof(cvOutputs9V)/sizeof(CVOutput)); i++)
//		cvOutputs9V[i].setup();
	
	pinMode(PATCH_DOWN_SW, INPUT_PULLUP);
	pinMode(PATCH_UP_SW, INPUT_PULLUP);
	
	pinMode(D_SPARE_1, INPUT_PULLUP);
	pinMode(D_SPARE_2, INPUT_PULLUP);
	pinMode(D_SPARE_3, INPUT_PULLUP);
	
	//cvOutputs5V[0].setValueProvider(&lfo);
}

void loop()
{
    long usNow = micros();
    display.loop(usNow);
	fsw1.loop(usNow);
	fsw2.loop(usNow);
	fsw3.loop(usNow);
	midi.loop(usNow);
	lfo.loop(usNow);
	displayTest.loop(usNow);
    
	for (uint8_t i=0; i<(sizeof(cvOutputs5V)/sizeof(CVOutput)); i++)
		cvOutputs5V[i].loop(usNow);
	
//	dac5V.setOutput(0, 0xc0);
	//	for (uint8_t i=0; i<(sizeof(cvOutputs9V)/sizeof(CVOutput)); i++)
	//		cvOutputs9V[i].loop(usNow);
}
