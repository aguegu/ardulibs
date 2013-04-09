/*
 * DigitalPin.h
 *
 *  Created on: 2012-3-14
 *      Author: Agu
 */

#ifndef DIGITALPIN_H_
#define DIGITALPIN_H_

#include "Arduino.h"

class DigitalPin
{
public:
	DigitalPin(uint8_t pin);
	virtual ~DigitalPin();
	void set(bool on);
	void turnOn();
	void turnOff();
	void reverse();
private:
	uint8_t _pin;
	bool _on;
};

#endif /* DIGITALPIN_H_ */
