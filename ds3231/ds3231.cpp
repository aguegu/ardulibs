/*
 * ds3231.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: agu
 */

#include "ds3231.h"

Ds3231::Ds3231() {
	twi.init();
	this->init();
	_datetime = (byte *) malloc(sizeof(byte) * 7);
	memset(_datetime, 0, 7);
}

Ds3231::~Ds3231() {
	free(_datetime);
}

void Ds3231::init() {
	this->setReg(0x0e, 0x9c);
	this->setReg(0x0f, 0x00);
}

byte Ds3231::setReg(byte reg, byte data) {
	uint8_t p[2];
	p[0] = reg;
	p[1] = data;
	return twi.writeTo(_address, p, 2, true, true);
}

byte Ds3231::getReg(byte reg) {
	byte c;
	twi.writeTo(_address, reg);
	twi.readFrom(_address, &c, 1, true);
	return c;
}

byte Ds3231::getHexData(byte index) {
	return _datetime[index] & 0x7f;
}

byte Ds3231::getDecData(byte index) {
	return Ds3231::hex2dec(_datetime[index] & 0x7f);
}

byte Ds3231::hex2dec(byte hex) {
	return ((hex >> 4) * 10 + (hex & 0x0f));
}

byte Ds3231::dec2hex(byte dec) {
	return (((dec / 10) << 4) + dec % 10);
}

void Ds3231::refresh(bool only_time) {
	byte j = only_time ? 3 : 7;
	for (byte i = 0; i < j; i++)
		_datetime[i] = this->getReg(i);
}

void Ds3231::setTime(byte hour, byte minute, byte second) {
	this->setReg(SECOND, dec2hex(second));
	this->setReg(MINUTE, dec2hex(minute));
	this->setReg(HOUR, dec2hex(hour));
}

void Ds3231::setDate(byte year, byte month, byte date, byte weekday) {
	this->setReg(WEEKDAY, weekday ? weekday : 7);
	this->setReg(DATE, dec2hex(date));
	this->setReg(MONTH, dec2hex(month));
	this->setReg(YEAR, dec2hex(year));
}

float Ds3231::getTemperature() {
	byte h = this->getReg(0x11);
	byte l = this->getReg(0x12);

	int t = makeWord(h, l);
	return t / 256.;
}
