/*
 * LiquidTemp.ino
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
 * example for Max6675 with Liquid Crystal Display 1602 output 
 */


#include "Max6675.h"
#include "LiquidCrystal.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/*
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 */

Max6675 ts(8, 9, 10);
// Max6675 module: SO on pin #8, SS on pin #9, CSK on pin #10 of Arduino UNO
// Other pins are capable to run this library, as long as digitalRead works on SO,
// and digitalWrite works on SS and CSK

void setup()
{
	lcd.begin(16, 2);

	ts.setOffset(0);
	// set offset for temperature measurement.
	// 1 stannds for -0.25 Celsius
}

void loop()
{
	lcd.clear();
	lcd.print(ts.getCelsius(), 2);
	lcd.write(' ');
	lcd.write(0xdf);
	lcd.write('C');

	lcd.setCursor(0, 1);
	lcd.print(ts.getFahrenheit(), 2);
	lcd.write(' ');
	lcd.write(0xdf);
	lcd.write('F');

	delay(500);
}
