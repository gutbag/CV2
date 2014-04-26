#include "Display.h"

static const uint8_t DIG_1 = 44;
static const uint8_t DIG_2 = 42;
static const uint8_t DIG_3 = 40;
static const uint8_t DIG_4 = 38;

static const uint8_t SEG_A = 39;
static const uint8_t SEG_B = 41;
static const uint8_t SEG_C = 37;
static const uint8_t SEG_D = 45;
static const uint8_t SEG_E = 35;
static const uint8_t SEG_F = 33;
static const uint8_t SEG_G = 43;
static const uint8_t SEG_DP = 31;

static const uint8_t LED_12 = 30;
static const uint8_t LED_3 = 13;

Display::Display()
: usLastChange(0), digitIndex(0)
{
    charMap['?'] = 0xCB;

    // init charMap with ?
    for (int i=0; i<sizeof(charMap); i++)
        charMap[i] = charMap['?'];
        
    charMap['?'] = 0xCB;
    charMap[' '] = 0x00;
    charMap['-'] = 0x02;
    charMap['_'] = 0x10;
    charMap['1'] = 0x60;
    charMap['2'] = 0xDA;
    charMap['3'] = 0xF2;
    charMap['4'] = 0x66;
    charMap['5'] = 0xB6;
    charMap['6'] = 0xBE;
    charMap['7'] = 0xE0;
    charMap['8'] = 0xFE;
    charMap['9'] = 0xE6;
    charMap['0'] = 0xFC;
    charMap['A'] = 0xEE;
    charMap['a'] = charMap['A'];
    charMap['b'] = 0x3E;
    charMap['B'] = charMap['b'];
    charMap['c'] = 0x1A;
    charMap['C'] = 0x9C;
    charMap['d'] = 0x7A;
    charMap['D'] = charMap['d'];
    charMap['E'] = 0x9E;
    charMap['e'] = charMap['E'];
    charMap['F'] = 0x8E;
    charMap['f'] = charMap['F'];
    charMap['r'] = 0x0A;
    charMap['R'] = charMap['r'];
}

void Display::setup()
{
    // all segments off
    pinMode(SEG_A, OUTPUT);
    digitalWrite(SEG_A, LOW);
    pinMode(SEG_B, OUTPUT);
    digitalWrite(SEG_B, LOW);
    pinMode(SEG_C, OUTPUT);
    digitalWrite(SEG_C, LOW);
    pinMode(SEG_D, OUTPUT);
    digitalWrite(SEG_D, LOW);
    pinMode(SEG_E, OUTPUT);
    digitalWrite(SEG_E, LOW);
    pinMode(SEG_F, OUTPUT);
    digitalWrite(SEG_F, LOW);
    pinMode(SEG_G, OUTPUT);
    digitalWrite(SEG_G, LOW);
    pinMode(SEG_DP, OUTPUT);
    digitalWrite(SEG_DP, LOW);
    
    // all digits off
    pinMode(DIG_1, OUTPUT);
    digitalWrite(DIG_1, HIGH);
    pinMode(DIG_2, OUTPUT);
    digitalWrite(DIG_2, HIGH);
    pinMode(DIG_3, OUTPUT);
    digitalWrite(DIG_3, HIGH);
    pinMode(DIG_4, OUTPUT);
    digitalWrite(DIG_4, HIGH);
    
    pinMode(LED_12, OUTPUT);
    digitalWrite(LED_12, LOW);
    pinMode(LED_3, OUTPUT);
    digitalWrite(LED_3, LOW);
}

void Display::loop(const unsigned long usNow)
{
    if ((usNow - usLastChange) >= 3000)
    {
        digitIndex++;
        if (digitIndex > sizeof(digits))
            digitIndex = 0;
        
        setDigit(digitIndex, digits[digitIndex]);
//        setDigit(0, 0x80);
        
        usLastChange = usNow;
    }
}

// display the 4 chars from s
void Display::set(const char* s)
{
    digits[0] = charMap[s[0]];
    digits[1] = charMap[s[1]];
    digits[2] = charMap[s[2]];
    digits[3] = charMap[s[3]];
}

void Display::setDigit(const uint8_t digitIndex, const byte value)
{
    digitalWrite(DIG_1, HIGH);
    digitalWrite(DIG_2, HIGH);
    digitalWrite(DIG_3, HIGH);
    digitalWrite(DIG_4, HIGH);
    
    digitalWrite(SEG_A, value & 0x80 ? HIGH : LOW);
    digitalWrite(SEG_B, value & 0x40 ? HIGH : LOW);
    digitalWrite(SEG_C, value & 0x20 ? HIGH : LOW);
    digitalWrite(SEG_D, value & 0x10 ? HIGH : LOW);
    digitalWrite(SEG_E, value & 0x08 ? HIGH : LOW);
    digitalWrite(SEG_F, value & 0x04 ? HIGH : LOW);
    digitalWrite(SEG_G, value & 0x02 ? HIGH : LOW);
    digitalWrite(SEG_DP, value & 0x01 ? HIGH : LOW);
    
    switch (digitIndex)
    {
        case 0:
            digitalWrite(DIG_1, LOW);
            break;
        case 1:
            digitalWrite(DIG_2, LOW);
            break;
        case 2:
            digitalWrite(DIG_3, LOW);
            break;
        case 3:
            digitalWrite(DIG_4, LOW);
            break;
        default:
            break;
    }
}

void Display::setColon(const boolean state)
{
	digitalWrite(LED_12, state ? HIGH : LOW);
}

void Display::setApostophe(const boolean state)
{
	digitalWrite(LED_3, state ? HIGH : LOW);
}
