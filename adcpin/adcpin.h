/*
 * adcpin.h
 *
 *  Created on: Aug 5, 2013
 *      Author: agu
 */

#ifndef ADCPIN_H_
#define ADCPIN_H_

#include "Arduino.h"

class AdcPin {
public:
	AdcPin(uint8_t pin);
	virtual ~AdcPin();

	static void setReference(uint8_t mode);
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.

	uint16_t read();

private:
	static uint8_t _ref;
	const uint8_t _channel;
};

#endif /* ADCPIN_H_ */
