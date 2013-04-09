/*
 * PinSet.h
 *
 *  Created on: 2012-6-28
 *      Author: Agu
 */

#ifndef PINSET_H_
#define PINSET_H_

#include "Arduino.h"

class PinSet
{
public:
	PinSet(byte count, uint8_t * pins);
	virtual ~PinSet();
	void setMode(uint8_t mode = INPUT);
	byte readByte();
	void writeByte(byte c);

	byte readKey();
	byte getFirstLow();
private:
	uint8_t * _pins;
	byte _length;

	bool _effective;
	byte _last_index;
	bool _rebound;
};

#endif /* PINSET_H_ */
