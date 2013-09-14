/*
 * ds18b20.h
 *
 *  Created on: Sep 14, 2013
 *      Author: agu
 */

#ifndef DS18B20_H_
#define DS18B20_H_

#include "OneWire.h"

class Ds18b20 {
public:
	Ds18b20(uint8_t pin);
	virtual ~Ds18b20();
	void init();
	void convert();
	int16_t getValue();

private:
	uint8_t _address[8];
	OneWire _ds;
};

#endif /* DS18B20_H_ */
