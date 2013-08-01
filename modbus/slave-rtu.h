/*
 * slave-rtu.h
 *
 *  Created on: Jul 30, 2013
 *      Author: agu
 */

#ifndef SLAVE_RTU_H_
#define SLAVE_RTU_H_

#include "Arduino.h"
#include "digitalpin.h"

class SlaveRtu {
public:
	SlaveRtu(HardwareSerial & usart);
	void init();
	virtual ~SlaveRtu();

	void handler();
	void onTimIrq();
private:
	HardwareSerial & _usart;
	bool _is_receiving;

	static const uint16_t _BUFF_LENGTH = 256;
	uint8_t _buff_rx[_BUFF_LENGTH];
	uint8_t _buff_tx[_BUFF_LENGTH];
};

#endif /* SLAVE_RTU_H_ */
