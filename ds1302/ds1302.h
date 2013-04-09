/*
 * Ds1302.h
 *
 *  Created on: 2012-3-11
 *      Author: Agu
 */

#ifndef DS1302_H_
#define DS1302_H_

#include "Arduino.h"

#define SECOND	0
#define MINUTE	1
#define HOUR 	2
#define DAY_OF_MONTH	3
#define MONTH	4
#define DAY_OF_WEEK	5
#define YEAR	6

class Ds1302
{
public:
	Ds1302(uint8_t pin_sclk, uint8_t pin_io, uint8_t pin_rst);
	virtual ~Ds1302();

	void setWritable(bool writable = true);
	void setCharger(bool enable = false, byte ds = 0x02, byte rs = 0x03);
	void halt(bool onHalt = false);
	void refresh(bool justTime = false);

	byte getHexData(byte index);
	byte getDecData(byte index);

	bool isHalt();

	void setTime(byte hour, byte minute, byte second);
	void setDate(byte year, byte month, byte date, byte weekday);

	static byte hex2dec(byte hex);
	static byte dec2hex(byte dec);

	void output(byte *p, byte length);

private:

	const uint8_t _pin_sclk;
	const uint8_t _pin_io;
	const uint8_t _pin_rst;

	byte _pDateTime[7];

	void write(byte cmd, byte data);
	byte read(byte cmd);

	void writeByte(byte c);
	byte readByte();
};

#endif /* DS1302_H_ */
