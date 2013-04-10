/*
 * Lpd6803.cpp
 *
 *  Created on: Dec 11, 2012
 *      Author: agu
 */

#include "lpd6803.h"

Lpd6803::Lpd6803(uint8_t pin_din, uint8_t pin_sck, byte length) :
		_reg_din(portOutputRegister(digitalPinToPort(pin_din)) ), _reg_sck(
				portOutputRegister(digitalPinToPort(pin_sck)) ), _mask_din(
				digitalPinToBitMask(pin_din)), _mask_sck(
				digitalPinToBitMask(pin_sck)), _mask_din_c(~_mask_din), _mask_sck_c(
				~_mask_sck), _length(length)
{
	pinMode(pin_din, OUTPUT);
	pinMode(pin_sck, OUTPUT);

	*_reg_din &= _mask_din_c;
	*_reg_sck &= _mask_sck_c;

	_cache = (word *) malloc(sizeof(word) * _length);

	for (byte i = 0; i < _length; i++)
	{
		_cache[i] = 0x8000;
	}
}

Lpd6803::~Lpd6803()
{
	free(_cache);
}

void Lpd6803::start()
{
	byte i = 32;
	while (i--)
	{
		pulse();
	}
}

void Lpd6803::end()
{
	byte i = _length;
	while (i--)
	{
		*_reg_din &= _mask_din_c;
		pulse();
	}
}

void Lpd6803::setColor(byte index, byte r, byte g, byte b)
{
	index %= _length;

	r &= 0x1f;
	g &= 0x1f;
	b &= 0x1f;

//	r = 0x1f - r;
//	g = 0x1f - g;
//	b = 0x1f - b;

	_cache[index] = 0x8000 | (word)g << 10 | (word)b << 5 | (word)r;
}

void Lpd6803::display()
{
	start();

	for (byte i = 0; i < _length; i++)
	{
		word tmp = _cache[i];
		for (byte j = 0; j < 16; j++)
		{
			tmp & 0x8000 ? *_reg_din |= _mask_din : *_reg_din &= _mask_din_c;
			pulse();
			tmp <<= 1;
		}
	}
	end();
}

void Lpd6803::pulse()
{
	*_reg_sck |= _mask_sck;
	*_reg_sck &= _mask_sck_c;
}

byte Lpd6803::getLength()
{
	return _length;
}
