/*
 * PwmPin.h
 *
 *  Created on: 2012-3-13
 *      Author: Agu
 */

#ifndef PWM_PIN_H_
#define PWM_PIN_H_

#include "Arduino.h"

class PwmPin
{
public:
	PwmPin(uint8_t pin);
	virtual ~PwmPin();
	void setPWM(byte pwm);
	void makeTone(unsigned int frequence, unsigned long duration);
	void mute();
private:
	uint8_t _pin;
};

#endif /* PWM_PIN_H_ */
