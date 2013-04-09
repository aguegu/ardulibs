/*
 * Lpd6803.h
 *
 *  Created on: Dec 11, 2012
 *      Author: agu
 */

#ifndef LPD6803_H_
#define LPD6803_H_

#include "Arduino.h"

class Lpd6803
{
public:
	Lpd6803(uint8_t pin_din, uint8_t pin_sck, byte length = 1);
	virtual ~Lpd6803();
	void display();
	void setColor(byte index, byte r, byte g, byte b);
	byte getLength();

private:

	volatile uint8_t * const _reg_din;
	volatile uint8_t * const _reg_sck;

	const byte _mask_din;
	const byte _mask_sck;

	const byte _mask_din_c;
	const byte _mask_sck_c;

	const byte _length;
	word * _cache;

	void start();
	void end();
	inline void pulse();
};

#endif /* LPD6803_H_ */
