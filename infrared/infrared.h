/*
 * Infrared.h
 *
 *  Created on: 2012-4-2
 *      Author: Agu
 */

#ifndef INFRARED_H_
#define INFRARED_H_

#include "Arduino.h"

class Infrared
{
public:

	Infrared(byte interruption_id, byte address = 0x00);
	virtual ~Infrared();
	byte getCode();
	byte getInterruptionID();
	void receive();
	void clear();
	void setRepeatEnable(bool en = false);

private:

	bool _repeat_en;
	const byte _interruption_id;
	const byte _pin;
	const byte _address;
	volatile byte _ir_code[4];
	volatile bool _repeat;
	bool inspect();
};

#endif /* INFRARED_H_ */
