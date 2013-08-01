/*
 * slave-rtu.cpp
 *
 *  Created on: Jul 30, 2013
 *      Author: agu
 */

#include "slave-rtu.h"

SlaveRtu::SlaveRtu(HardwareSerial & usart) :
		_usart(usart) {
	_is_receiving = true;
}

SlaveRtu::~SlaveRtu() {

}

void SlaveRtu::init() {
#if defined(TCCR2)
	TIMER_CS(TCCR2, 2, TIMER_WITHOUT_EXT_CLK_CS_64);
	TIMER_2BIT_WAVEFORM(2, TIMER_2BIT_WAVEFORM_PCPWM);
	sbi(TIMSK2, OCIE2);
#elif defined(TCCR2A) && defined(TCCR2B)
	TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_NUL);
	TIMER_3BIT_WAVEFORM(2, TIMER_3BIT_WAVEFORM_CTC);
	// (11/19200) * 3.5 / (1/(16000000/128))
	OCR2A = 250;
	sbi(TIMSK2, OCIE2A);
#else
#error Timer 2 reg not found
#endif

	_usart.begin(19200, SERIAL_8E1);
}

void SlaveRtu::handler() {

	extern DigitalPin led;
	static uint16_t length_rx = 0;

	if (_usart.available()) {
		_buff_rx[length_rx++] = _usart.read();
		TCNT2 = 0;
		sbi(TIMSK2, OCIE2A);
		TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_128);
		led.set(HIGH);
	}

	if (_is_receiving == false) {
		led.set(LOW);

		memset(_buff_tx, 0, _BUFF_LENGTH);
		_usart.write(length_rx);

		_is_receiving = true;
		length_rx = 0;
	}
}

void SlaveRtu::onTimIrq() {
	TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_NUL);
	cbi(TIMSK2, OCIE2A);
	_is_receiving = false;
}

ISR(TIMER2_COMPA_vect) {
	extern SlaveRtu node;
	node.onTimIrq();
}
