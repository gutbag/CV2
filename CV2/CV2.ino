#include "Arduino.h"
#include "Display.h"
#include "DAC.h"
#include "Footswitch.h"
#include "Opto.h"
#include "Freeze.h"
#include "MIDI.h"
#include "CVOutput.h"
#include "LFO.h"
#include "Patch.h"
#include "DisplayTest.h"
#include "Expression.h"
#include "Switch.h"
#include "CV2.h"
#include "EEPROM.h"
#include "EnvelopeFollower.h"
#include "Ramp.h"

EEPROM eeprom;
MIDI midi;
Display display;
DisplayTest displayTest;
DAC dac5V(CS_PIN_5V_DAC);
DAC dac9V(CS_PIN_9V_DAC);
CVOutput cvOutputs5V[8] =
{
	{dac5V, 0, 0},
	{dac5V, 1, 1},
	{dac5V, 2, 2},
	{dac5V, 3, 3},
	{dac5V, 4, 4},
	{dac5V, 5, 5},
	{dac5V, 6, 6},
	{dac5V, 7, 7}
};
CVOutput cvOutputs9V[6] =
{
	{dac9V, 0, 8},
	{dac9V, 1, 9},
	{dac9V, 2, 10},
	{dac9V, 3, 11},
	{dac9V, 4, 12},
	{dac9V, 5, 13}
};
Footswitch footswitches[3] = {
	{0, FSW_1, FSW_LED1},
	{1, FSW_2, FSW_LED2},
	{2, FSW_3, FSW_LED3}
};
Freeze freezes[2] = {
	{OPTO_1, 0},
	{OPTO_2, 1},
};
LFO lfo;
Switch patchDownSwitch(PATCH_DOWN_SW);
Switch patchUpSwitch(PATCH_UP_SW);
Patch patch(patchDownSwitch, patchUpSwitch);
Expression expr1(EXPR_1_PIN, 0);
Expression expr2(EXPR_2_PIN, 1);
EnvelopeFollower envelopeFollower(LPF_ENV_IN);
Ramp ramp;

void setup()
{
	Serial.begin(115200);
	
	eeprom.setup();
	display.setup();
	midi.setup();
	dac5V.setup();
//	dac9V.setup();
	for (uint8_t i=0; i<3; i++)
		footswitches[i].setup();
	patchDownSwitch.setup();
	patchUpSwitch.setup();
	lfo.setup();
	expr1.setup();
	expr2.setup();
	envelopeFollower.setup();
	ramp.setup();
	
	displayTest.enable(false);
	displayTest.setup();
	
	for (uint8_t i=0; i<2; i++)
		freezes[i].setup();
	
	for (uint8_t i=0; i<4; i++)
		optos[i].setup();
	
	for (uint8_t i=0; i<(sizeof(cvOutputs5V)/sizeof(CVOutput)); i++)
		cvOutputs5V[i].setup();
	
//	for (uint8_t i=0; i<(sizeof(cvOutputs9V)/sizeof(CVOutput)); i++)
//		cvOutputs9V[i].setup();

	patch.setup();
	
	pinMode(D_SPARE_1, INPUT_PULLUP);
	pinMode(D_SPARE_2, INPUT_PULLUP);
	pinMode(D_SPARE_3, INPUT_PULLUP);
	
	// for now, enable permanently
	eeprom.writeEnable(true);
}

void loop()
{
    long usNow = micros();
    display.loop(usNow);
	displayTest.loop(usNow);
	for (uint8_t i=0; i<3; i++)
		footswitches[i].loop(usNow);
	patchDownSwitch.loop(usNow);
	patchUpSwitch.loop(usNow);
	midi.loop(usNow);
	lfo.loop(usNow);
	expr1.loop(usNow);
	expr2.loop(usNow);
	envelopeFollower.loop(usNow);
	ramp.loop(usNow);
	for (uint8_t i=0; i<2; i++)
		freezes[i].loop(usNow);
	for (uint8_t i=0; i<4; i++)
		optos[i].loop(usNow);
	   
	for (uint8_t i=0; i<(sizeof(cvOutputs5V)/sizeof(CVOutput)); i++)
		cvOutputs5V[i].loop(usNow);
	
	//	for (uint8_t i=0; i<(sizeof(cvOutputs9V)/sizeof(CVOutput)); i++)
	//		cvOutputs9V[i].loop(usNow);
	
	patch.loop(usNow);
}
