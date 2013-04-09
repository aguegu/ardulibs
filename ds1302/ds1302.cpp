/*
 * Ds1302.cpp
 *
 *  Created on: 2012-3-11
 *      Author: Agu
 */

#include "ds1302.h"

Ds1302::Ds1302(uint8_t pin_sclk, uint8_t pin_io, uint8_t pin_rst) :
		_pin_sclk(pin_sclk), _pin_io(pin_io), _pin_rst(pin_rst)
{
	pinMode(_pin_sclk, OUTPUT);
	pinMode(_pin_rst, OUTPUT);

	digitalWrite(_pin_sclk, LOW);
	digitalWrite(_pin_rst, LOW);
}

void Ds1302::writeByte(byte c)
{
	pinMode(_pin_io, OUTPUT);

	for (byte i = 0; i < 8; i++)
	{
		digitalWrite(_pin_io, bit_is_set(c, i));

		digitalWrite(_pin_sclk, LOW);
		digitalWrite(_pin_sclk, HIGH);
	}
}

byte Ds1302::readByte()
{
	byte data = 0;
	pinMode(_pin_io, INPUT);

	for (int i = 0; i < 8; ++i)
	{
		digitalWrite(_pin_sclk, HIGH);
		digitalWrite(_pin_sclk, LOW);

		bitWrite(data, i, digitalRead(_pin_io));
	}
	return data;
}

void Ds1302::write(byte cmd, byte data)
{
	digitalWrite(_pin_sclk, LOW);
	digitalWrite(_pin_rst, HIGH);

	this->writeByte(cmd & 0xfe);
	this->writeByte(data);

	digitalWrite(_pin_rst, LOW);
	digitalWrite(_pin_sclk, LOW);
}

byte Ds1302::read(byte cmd)
{
	digitalWrite(_pin_sclk, LOW);
	digitalWrite(_pin_rst, HIGH);

	this->writeByte(cmd | 0x01);
	byte c = this->readByte();

	digitalWrite(_pin_rst, LOW);
	digitalWrite(_pin_sclk, LOW);
	return c;
}

////////////////////////////////////////////////////////

void Ds1302::setWritable(bool writable)
{
	this->write(0x8e, !writable);
}

void Ds1302::setCharger(bool enable, byte ds, byte rs)
{
	byte c;
	c = enable ? 0xa0 : 0x00;
	c |= (ds & 0x03) << 2;
	c |= rs & 0x03;
	this->write(0x90, c);
}

void Ds1302::halt(bool onHalt)
{
	byte second = this->read(0x81);
	bitWrite(second, 7, onHalt);
	this->write(0x80, second);
}

////////////////////////////////////////////////////////

void Ds1302::refresh(bool justTime)
{
	byte length = justTime ? 3 : 7;
	digitalWrite(_pin_sclk, LOW);
	digitalWrite(_pin_rst, HIGH);

	this->writeByte(0xbf);

	for (byte i = 0; i < length; i++)
		_pDateTime[i] = this->readByte();

	digitalWrite(_pin_rst, LOW);
	digitalWrite(_pin_sclk, LOW);
}

////////////////////////////////////////////////////////

byte Ds1302::getHexData(byte index)
{
	return _pDateTime[index] & 0x7f;
}

byte Ds1302::getDecData(byte index)
{
	return Ds1302::hex2dec(_pDateTime[index] & 0x7f);
}

void Ds1302::output(byte *p, byte length)
{
	for (byte i = 0; i < 7 && i < length; i++)
	{
		p[i] = Ds1302::hex2dec(_pDateTime[i]);
	}
}

bool Ds1302::isHalt()
{
	byte second = this->read(0x81);
	return bit_is_set(second, 7);
}

void Ds1302::setTime(byte hour, byte minute, byte second)
{
	this->setWritable(true);

	second %= 60;
	byte sec = Ds1302::dec2hex(second);
	//bitWrite(sec, 7, isHalt);
	this->write(0x80, sec);

	minute %= 60;
	this->write(0x82, Ds1302::dec2hex(minute));

	hour %= 24;
	this->write(0x84, Ds1302::dec2hex(hour));

	this->setWritable(false);

	this->refresh(false);
}

void Ds1302::setDate(byte year, byte month, byte date, byte weekday)
{
	this->setWritable(true);
	date %= 32;
	date = dec2hex(date);
	this->write(0x86, date);

	month %= 13;
	this->write(0x88, Ds1302::dec2hex(month));

	weekday %= 8;
	if (weekday == 0)
		weekday = 7;
	this->write(0x8a, weekday);

	year %= 100;
	this->write(0x8c, Ds1302::dec2hex(year));

	this->setWritable(false);

	this->refresh(false);
}

Ds1302::~Ds1302()
{

}

byte Ds1302::hex2dec(byte hex)
{
	return ((hex >> 4) * 10 + (hex & 0x0f));
}

byte Ds1302::dec2hex(byte dec)
{
	return (((dec / 10) << 4) + dec % 10);
}
