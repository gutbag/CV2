#include <Arduino.h>

class Display
{
public:
    Display();
    virtual ~Display() {}
    
    void set(const char* s);
    
    void setup();
    void loop(long usNow);
    
private:
    byte charMap[128]; // TODO: make const?
    void setDigit(const uint8_t digitIndex, const byte value);
    byte digits[4];
    uint8_t digitIndex;
    unsigned long usLastChange;
};