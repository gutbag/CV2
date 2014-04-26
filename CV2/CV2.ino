//
// EmbedExperiment1
//
// Description of the project
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author	 	Mark Lamb
// 				Mark Lamb
//
// Date			20/04/2014 12:24
// Version		<#version#>
//
// Copyright	© Mark Lamb, 2014
// License		<#license#>
//
// See			ReadMe.txt for references
//

// Core library for code-sense

#include "Arduino.h"
#include <SPI.h>
#include "Display.h"

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

#define CS_PIN 10

Display display;

void allSegmentsOff();

void setup()
{
    Serial.begin(115200);
    Serial1.begin(31250);
    
    Serial.println("some output");
    
    // optos
    pinMode(OPTO_1, OUTPUT);
    digitalWrite(OPTO_1, LOW);
    pinMode(OPTO_2, OUTPUT);
    digitalWrite(OPTO_2, LOW);
    pinMode(OPTO_3, OUTPUT);
    digitalWrite(OPTO_3, LOW);
    pinMode(OPTO_4, OUTPUT);
    digitalWrite(OPTO_4, LOW);
    pinMode(OPTO_5, OUTPUT);
    digitalWrite(OPTO_5, LOW);
    pinMode(OPTO_6, OUTPUT);
    digitalWrite(OPTO_6, LOW);
    
    display.setup();
    
    pinMode(PATCH_DOWN_SW, INPUT_PULLUP);
    pinMode(PATCH_UP_SW, INPUT_PULLUP);
    
    pinMode(FSW_1, INPUT_PULLUP);
    pinMode(FSW_2, INPUT_PULLUP);
    pinMode(FSW_3, INPUT_PULLUP);
    
    // LED pins input for now
    pinMode(FSW_LED1, INPUT_PULLUP);
    pinMode(FSW_LED2, INPUT_PULLUP);
    pinMode(FSW_LED3, INPUT_PULLUP);
    
    pinMode(D_SPARE_1, INPUT_PULLUP);
    pinMode(D_SPARE_2, INPUT_PULLUP);
    pinMode(D_SPARE_3, INPUT_PULLUP);

    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);
    SPI.begin();
    SPI.setClockDivider(84);
    SPI.setDataMode(SPI_MODE3);
    SPI.setBitOrder(MSBFIRST);
}

void loopOptos()
{
    digitalWrite(OPTO_2, LOW);
    delay(1000);
    digitalWrite(OPTO_2, HIGH);
    delay(1000);
}
//
//void loop()
//{
//    if (digitalRead(PATCH_DOWN_SW) == LOW)
//        display.setDelay(50 * 1000);
//    else if (digitalRead(PATCH_UP_SW) == LOW)
//        display.setDelay(500 * 1000);
//    else
//        display.setDelay(200 * 1000);
//    
//    display.loop();
//}

void on(byte channel, byte note, byte velocity)
{
	Serial1.write(0x90 | (channel-1));
	Serial1.write(note);
	Serial1.write(velocity);
}

void off(byte channel, byte note, byte velocity)
{
	Serial1.write(0x80 | (channel-1));
	Serial1.write(note);
	Serial1.write(velocity);
}


void loopMIDI()
{
//    Serial1.write(0xAA);
//    Serial1.write(0x55);
    
    on(1, 48, 0x45);
    delay(1000);
    off(1, 48, 0);
    delay(1000);
    
    size_t n = Serial1.available();
    if (n > 0)
    {
        Serial.print("got ");
        Serial.println(n);
        char buf[32];
        Serial1.readBytes(buf, n);
        for (int i=0; i<n; i++)
        {
            Serial.print(buf[i], HEX);
        }
    }
}

char c[] = {'?', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'C', 'd', 'e', 'f', 'E', 'r', 'r', ' ', '-', '_'};
int i = 0;
unsigned long t = 0;

void loop()
{
    long usNow = micros();
    display.loop(usNow);
    
    if (usNow - t > 600000)
    {
        i++;
        if (i > sizeof(c)-4)
            i=0;
        
        display.set(&c[i]);
        t = usNow;
    }
    
}
