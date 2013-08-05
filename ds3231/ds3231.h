/*
 * ds3231.h
 *
 *  Created on: Apr 8, 2013
 *      Author: agu
 */

#ifndef DS3231_H_
#define DS3231_H_

#include "Arduino.h"
#include "twi/twi.h"

#define SECOND	0
#define MINUTE	1
#define HOUR 	2
#define DATE	4
#define MONTH	5
#define WEEKDAY	3
#define YEAR	6

class Ds3231 {

public:
	Ds3231();
	virtual ~Ds3231();
	void init();

	void setTime(byte hour, byte minute, byte second);
	void setDate(byte year, byte month, byte date, byte weekday);

	byte getHexData(byte index);
	byte getDecData(byte index);

	static byte hex2dec(byte hex);
	static byte dec2hex(byte dec);

	void refresh(bool only_time = false);
	float getTemperature();

private:
	static const byte _address = 0x68;
	byte setReg(byte reg, byte data);
	byte getReg(byte reg);
	byte *_datetime;
};

#endif /* DS3231_H_ */
