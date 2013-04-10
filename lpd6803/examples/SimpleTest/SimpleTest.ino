#include "Lpd6803Examples.h"
#include "lpd6803.h"

Lpd6803 lpd(12, 13, 10);
// DIN - #12
// SCK - #13
// 1 pcs of lpd6803 in serial

void setup() {
}

void loop() {
	for (byte j = 0; j < 0x40; j++) {
		for (byte i = 0; i < lpd.getLength(); i++)
			lpd.setColor(i, j < 0x20 ? j : 0x3f - j, 0, 0);

		lpd.display();
		delay(50);
	}

	for (byte j = 0; j < 0x40; j++) {
		for (byte i = 0; i < lpd.getLength(); i++)
			lpd.setColor(i, 0, j < 0x20 ? j : 0x3f - j, 0);

		lpd.display();
		delay(50);
	}

	for (byte j = 0; j < 0x40; j++) {
		for (byte i = 0; i < lpd.getLength(); i++)
			lpd.setColor(i, 0, 0, j < 0x20 ? j : 0x3f - j);

		lpd.display();
		delay(50);
	}

	for (byte j = 0; j < lpd.getLength(); j++) {
		for (byte i = 0; i < lpd.getLength(); i++) {
			lpd.setColor(i, i == j ? 0x1f : 0x00, 0, 0);
		}
		lpd.display();
		delay(200);
	}

	for (byte j = 0; j < lpd.getLength(); j++) {
		for (byte i = 0; i < lpd.getLength(); i++) {
			lpd.setColor(i, 0, i == j ? 0x1f : 0x00, 0);
		}
		lpd.display();
		delay(200);
	}

	for (byte j = 0; j < lpd.getLength(); j++) {
		for (byte i = 0; i < lpd.getLength(); i++) {
			lpd.setColor(i, 0, 0, i == j ? 0x1f : 0x00);
		}
		lpd.display();
		delay(200);
	}

}
