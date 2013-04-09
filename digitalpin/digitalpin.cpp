/*
 * DigitalPin.cpp
 *
 *  Created on: 2012-3-14
 *      Author: Agu
 */

#include "digitalpin.h"

DigitalPin::DigitalPin(uint8_t pin)
{
	// TODO Auto-generated constructor stub
	_pin = pin;
	_on = false;
	pinMode(_pin, OUTPUT);
	this->turnOff();
}

DigitalPin::~DigitalPin()
{
	// TODO Auto-generated destructor stub
}

void DigitalPin::turnOn()
{
	this->set(true);
}

void DigitalPin::turnOff()
{
	this->set(false);
}

void DigitalPin::set(bool on)
{
	_on = on;
	digitalWrite(_pin, _on);
}

void DigitalPin::reverse()
{
	_on = !_on;
	this->set(_on);
}
