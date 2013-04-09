/*
 * PinSet.cpp
 *
 *  Created on: 2012-6-28
 *      Author: Agu
 */

#include "pinset.h"

PinSet::PinSet(byte count, uint8_t * pins)
{
	_length = count;
	_pins = (uint8_t *) malloc(sizeof(uint8_t) * _length);

	for (byte i = 0; i < _length; i++)
	{
		_pins[i] = pins[i];
	}

	_effective = false;
	_rebound = true;
	_last_index= 255;
}

void PinSet::setMode(uint8_t mode)
{
	for (byte i = 0; i < _length; i++)
	{
		pinMode(_pins[i], mode);
	}
}

byte PinSet::readByte()
{
	byte value = 0x00;
	for (byte i = 0; i < _length && i < 8; i++)
	{
		if (digitalRead(_pins[i]))
			value |= _BV(i);
	}
	return value;
}

void PinSet::writeByte(byte c)
{
	for (byte i = 0; i < _length && i < 8; i++)
	{
		digitalWrite(_pins[i], bitRead(c, i));
	}
}

byte PinSet::getFirstLow()
{
	byte i = 0;
	while (i < _length)
	{
		if (digitalRead(_pins[i]) == LOW)
			break;
		i++;
	}
	if (i == _length)
		i = 255;
	return i;
}

byte PinSet::readKey()
{
	byte index = this->getFirstLow();
	if (index < 255)
	{
		if (index == _last_index && _rebound == true)
			_effective = true;
	}
	else
	{
		_effective = false;
		_rebound = true;
	}

	_last_index = index;

	if (_effective)
	{
		_rebound = false;
		_effective = false;
		return index;
	}
	else
		return 255;
}

PinSet::~PinSet()
{
	free(_pins);
}

