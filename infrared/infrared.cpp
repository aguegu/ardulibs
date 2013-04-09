/*
 * Infrared.cpp
 *
 *  Created on: 2012-4-2
 *      Author: Agu
 */

#include "infrared.h"

Infrared::Infrared(byte interruption_id, byte address) :
		_interruption_id(interruption_id), _pin(_interruption_id + 2), _address(
				address)
{
	this->clear();
	_repeat = false;
	_repeat_en = false;
	this->setRepeatEnable();
}

void Infrared::setRepeatEnable(bool en)
{
	_repeat_en = en;
}

byte Infrared::getCode()
{
	static byte last_code = 0x00;
	byte code = 0x00;

	if (this->inspect())
	{
		code = _ir_code[2];
		last_code = code;
	}

	if (_repeat && _repeat_en)
	{
		code = last_code;
		_repeat = false;
	}

	//this->clear();

	return code;
}

byte Infrared::getInterruptionID()
{
	return _interruption_id;
}

void Infrared::receive()
{
	static uint32_t last_stamp = 0;
	static byte last_state = HIGH;
	static byte index = 0;
	static byte last_len = 0;

	bool state = digitalRead(_pin);
	uint32_t stamp = micros();
	uint32_t span = stamp - last_stamp;

	byte len = span >> 6;
	if (last_state == HIGH)
	{
		if (last_len >= 0x06 && last_len < 0x0c)
		{
			byte i = index >> 3;
			if (i > 3)
				i = 3;
			byte j = index & 0x07;

			if (len < 0x10)
				bitWrite(_ir_code[i], j, 0);
			else if (len < 0x20)
				bitWrite(_ir_code[i], j, 1);

			index++;
		}
		else
		{
			index = 0;
			//this->clear();
			_repeat = len >= 0x1e && len <= 0x22;
		}
	}

	last_state = state;
	last_stamp = stamp;
	last_len = len;
}

bool Infrared::inspect()
{
	byte address_c = ~_address;
	byte code_c = ~_ir_code[3];

	bool av = (_ir_code[0] == _address) && (_ir_code[1] == address_c)
			&& (_ir_code[2] == code_c);

	return av;
}

void Infrared::clear()
{
	for (byte i = 0; i < 4; i++)
		_ir_code[i] = 0;
}

Infrared::~Infrared()
{
}
