/*
 * adcpin.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: agu
 */

#include "adcpin/adcpin.h"

uint8_t AdcPin::_ref = DEFAULT;

AdcPin::AdcPin(uint8_t pin) :
		_channel(analogPinToChannel(pin)) {

}

AdcPin::~AdcPin() {

}

uint16_t AdcPin::read() {
#if defined(ADCSRB) && defined(MUX5)
	ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((_channel >> 3) & 0x01) << MUX5);
#endif

	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
#if defined(ADMUX)
	ADMUX = (_ref << 6) | (_channel & 0x07);
#endif

	// without a delay, we seem to read from the wrong channel
	//delay(1);

#if defined(ADCSRA) && defined(ADCL)
	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	uint8_t low  = ADCL;
	uint8_t high = ADCH;
#else
	// we dont have an ADC, return 0
	low  = 0;
	high = 0;
#endif

	// combine the two bytes
	return make16(high, low);
}

void AdcPin::setReference(uint8_t mode) {
	_ref = mode;
}
