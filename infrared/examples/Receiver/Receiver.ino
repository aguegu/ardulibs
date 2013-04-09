#include "infrared.h"

DrvHd44780 lcd(8, 9, 10, 11, 12, 13);

Infrared ir(0);

void scan() {
	ir.receive();
}

void setup() {
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);

	digitalWrite(3, LOW);
	digitalWrite(4, HIGH);
	lcd.init();
	attachInterrupt(ir.getInterruptionID(), scan, CHANGE);
}

void loop() {
	lcd.printf("%02x", ir.getCode());
	lcd.putCache();
	//ir.clear();
		delay(100);
		}
