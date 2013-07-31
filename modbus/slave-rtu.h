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
	SlaveRtu(HardwareSerial usart);
	void init();
	virtual ~SlaveRtu();

private:
	 HardwareSerial const & _usart;
	bool _is_receiving;
};

#endif /* SLAVE_RTU_H_ */
