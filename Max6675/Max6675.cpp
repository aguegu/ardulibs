/*
 * Max6675.cpp
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

#include "Max6675.h"

Max6675::Max6675(uint8_t pin_so, uint8_t pin_cs, uint8_t pin_clk, int offset) :
		_pin_so(pin_so), _pin_cs(pin_cs), _pin_clk(pin_clk)
{
	pinMode(_pin_clk, OUTPUT);
	pinMode(_pin_cs, OUTPUT);
	pinMode(_pin_so, INPUT);

	digitalWrite(_pin_cs, HIGH);

	_offset = offset;
}

void Max6675::setOffset(int offset)
{
	_offset = offset;
}

int Max6675::getValue()
{
	digitalWrite(_pin_cs, LOW);
	digitalWrite(_pin_clk, LOW);
	byte cH = shiftIn(_pin_so, _pin_clk, MSBFIRST);
	byte cL = shiftIn(_pin_so, _pin_clk, MSBFIRST);

	int temperature = makeWord(cH, cL);
	digitalWrite(_pin_cs, HIGH);
	return (temperature >> 3) - _offset;
}

float Max6675::getCelsius()
{
	float celsius = (float) getValue() / 4.0;
	return celsius;
}

float Max6675::getFahrenheit()
{
	float fahrenheit = (float) getValue() / 4.0 * 9 / 5.0 + 32.;
	return fahrenheit;
}

float Max6675::getKelvin()
{
	float kelvin = (float) getValue() / 4.0 + 235.15;
	return kelvin;
}

Max6675::~Max6675()
{
	// TODO Auto-generated destructor stub
}

