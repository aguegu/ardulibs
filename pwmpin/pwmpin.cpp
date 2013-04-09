/*
 * AnalogPin.cpp
 *
 *  Created on: 2012-3-13
 *      Author: Agu
 */

#include "pwmpin.h"

PwmPin::PwmPin(uint8_t pin)
{
	// TODO Auto-generated constructor stub
	_pin = pin;
	pinMode(_pin, OUTPUT);
}

void PwmPin::setPWM(byte pwm)
{
	analogWrite(_pin, pwm);
}


void PwmPin::makeTone(unsigned int frequence, unsigned long duration)
{
	tone(_pin, frequence, duration);
}

void PwmPin::mute()
{
	digitalWrite(_pin, HIGH);
}
PwmPin::~PwmPin()
{
	// TODO Auto-generated destructor stub
}


