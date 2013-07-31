/*
 * slave-rtu.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: agu
 */

#include "slave-rtu.h"

SlaveRtu::SlaveRtu(HardwareSerial usart) :
		_usart(usart) {
	_is_receiving = true;
	TCNT2 = 0;
}

SlaveRtu::~SlaveRtu() {

}

void SlaveRtu::init() {
#if defined(TCCR2)
	TIMER_CS(TCCR2, 2, TIMER_WITHOUT_EXT_CLK_CS_64);
	TIMER_2BIT_WAVEFORM(2, TIMER_2BIT_WAVEFORM_PCPWM);
	sbi(TIMSK2, OCIE2);
#elif defined(TCCR2A) && defined(TCCR2B)
	TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_128);
	TIMER_3BIT_WAVEFORM(2, TIMER_3BIT_WAVEFORM_CTC);
	OCR2A = 0xf8;
	sbi(TIMSK2, OCIE2A);
#else
#error Timer 2 reg not found
#endif
}

ISR(TIMER2_COMPA_vect) {
	extern DigitalPin led;
	led.toggle();
}
