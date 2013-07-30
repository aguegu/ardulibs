/*
 * DigitalPin.h
 *
 *  Created on: 2012-3-14
 *      Author: Agu
 */

#ifndef DIGITALPIN_H_
#define DIGITALPIN_H_

#include "Arduino.h"

class DigitalPin {
public:
	DigitalPin(uint8_t pin, uint8_t mode);
	virtual ~DigitalPin();
	void set(bool on);
	void toggle();
	bool read();

private:
	const uint8_t _mask;
	volatile uint8_t * const _out;
	volatile uint8_t * const _in;
};

#endif /* DIGITALPIN_H_ */
