#include "Opto.h"

Opto::Opto(const uint8_t aPin)
: pin(aPin)
{
	
}

Opto::~Opto()
{
	
}

void Opto::setup()
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

void Opto::on()
{
	digitalWrite(pin, HIGH);
}

void Opto::off()
{
	digitalWrite(pin, LOW);
}
