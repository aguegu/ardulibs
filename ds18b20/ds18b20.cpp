/*
 * ds18b20.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: agu
 */

#include "ds18b20/ds18b20.h"

Ds18b20::Ds18b20(uint8_t pin) :
		_ds(pin) {

}

Ds18b20::~Ds18b20() {

}

void Ds18b20::init() {
	_ds.search(_address);
}

void Ds18b20::convert() {
	_ds.reset();
	_ds.select(_address);
	_ds.write(0x44, 1);
}

int16_t Ds18b20::getValue() {
	uint8_t data[2];
	_ds.reset();
	_ds.select(_address);
	_ds.write(0xBE);         // Read Scratchpad
	for (uint8_t i = 0; i < 2; i++) {           // we need 9 bytes
		data[i] = _ds.read();
	}
	return make16(data[1], data[0]);
}
