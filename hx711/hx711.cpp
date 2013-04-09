/*
 * Hx711.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: agu
 */

#include "hx711.h"

Hx711::Hx711(uint8_t pin_dout, uint8_t pin_slk) :
		_pin_dout(pin_dout), _pin_slk(pin_slk)
{
	pinMode(_pin_slk, OUTPUT);
	pinMode(_pin_dout, INPUT);

	digitalWrite(_pin_slk, HIGH);
	delayMicroseconds(100);
	digitalWrite(_pin_slk, LOW);

	averageValue();
	this->setOffset(averageValue());
	this->setScale();
}

Hx711::~Hx711()
{

}

long Hx711::averageValue(byte times)
{
	long sum = 0;
	for (byte i = 0; i < times; i++)
	{
		sum += getValue();
	}

	return sum / times;
}

long Hx711::getValue()
{
	byte data[3];

	while (digitalRead(_pin_dout))
		;

	for (byte j = 3; j--;)
	{
		for (char i = 8; i--;)
		{
			digitalWrite(_pin_slk, HIGH);
			bitWrite(data[j], i, digitalRead(_pin_dout));
			digitalWrite(_pin_slk, LOW);
		}
	}

	digitalWrite(_pin_slk, HIGH);
	digitalWrite(_pin_slk, LOW);

	data[2] ^= 0x80;

	return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8)
			| (uint32_t) data[0];
}

void Hx711::setOffset(long offset)
{
	_offset = offset;
}

void Hx711::setScale(float scale)
{
	_scale = scale;
}

float Hx711::getGram()
{
	long val = (averageValue() - _offset);
	return (float) val / _scale;
}
