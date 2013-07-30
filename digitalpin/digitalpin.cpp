/*
 * DigitalPin.cpp
 *
 *  Created on: 2012-3-14
 *      Author: Agu
 */

#include "digitalpin.h"

DigitalPin::DigitalPin(uint8_t pin, uint8_t mode) :
		_mask( digitalPinToBitMask(pin)), _out(
		portOutputRegister(digitalPinToPort(pin))), _in(
				portInputRegister(digitalPinToPort(pin))) {
	volatile uint8_t * reg = portModeRegister(digitalPinToPort(pin));
	bitWriteMask(*reg, _mask, mode & 0x01);
	bitWriteMask(*_out, _mask, mode & 0x02);
}

DigitalPin::~DigitalPin() {
}

void DigitalPin::set(bool on) {
	bitWriteMask(*_out, _mask, on);
}

void DigitalPin::toggle() {
	bitToggleMask(*_out, _mask);
}

bool DigitalPin::read() {
	return bitReadMask(*_in, _mask);
}
