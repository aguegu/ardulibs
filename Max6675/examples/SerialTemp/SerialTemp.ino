/*
 * SerialTemp.ino
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
 *
 * example for Max6675 with Serial output 
 */


#include "Max6675.h"

Max6675 ts(8, 9, 10);
// Max6675 module: SO on pin #8, SS on pin #9, CSK on pin #10 of Arduino UNO
// Other pins are capable to run this library, as long as digitalRead works on SO,
// and digitalWrite works on SS and CSK

void setup()
{
	ts.setOffset(0);
	// set offset for temperature measurement.
	// 1 stannds for 0.25 Celsius

	Serial.begin(9600);
}


void loop()
{
	Serial.print(ts.getCelsius(), 2);
	Serial.print(" C / ");
	Serial.print(ts.getFahrenheit(), 2);
	Serial.print(" F / ");
	Serial.print(ts.getKelvin(), 2);
	Serial.print(" K\n");
	delay(300);
}
