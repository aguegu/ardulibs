/*
 * Max6675.h
 *
 * Arduino Library of Max6675 Cold-Junction-Compensated K-Thermocouple-
 * to-Digital Converter (0°C to +1024°C)
 *
 * Max6675 official page: http://www.maximintegrated.com/datasheet/index.mvp/id/3149
 *
 * author: Weihong Guan (@aGuegu)
 * host on: https://github.com/aguegu/Arduino/tree/master/libraries/Max6675
 * e-mail: weihong.guan@gmail.com
 * created on: 2012-8-6
 * update on: 2011-12-03
 *
 * module designer: syyyd.com
 */

#ifndef MAX6675_H_
#define MAX6675_H_

#include "Arduino.h"

class Max6675
{
public:
	Max6675(uint8_t pin_dt, uint8_t pin_ss, uint8_t pin_clk, int offset = 0);
	virtual ~Max6675();
	float getCelsius();
	float getFahrenheit();
	float getKelvin();
	int getValue();
	void setOffset(int offset);

private:
	const uint8_t _pin_so;
	const uint8_t _pin_cs;
	const uint8_t _pin_clk;
	int _offset;


};

#endif /* MAX6675_H_ */
