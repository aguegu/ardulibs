#include "digitalpin.h"
DigitalPin sender(A0);

void delaySpan(uint32_t span) {
	uint32_t beginning = micros();
	uint32_t ending = beginning + (span << 6);
	while (micros() < ending)
		;
}

void sendBegin() {
	sender.turnOff();
	delaySpan(0x80);
	sender.turnOn();
	delaySpan(0x40);
}

void sendEnd() {
	sender.turnOff();
	delaySpan(0x08);
	sender.turnOn();
}

void sendByte(byte data) {
	for (byte i = 0; i < 8; i++) {
		sender.turnOff();
		delaySpan(0x08);
		sender.turnOn();
		delaySpan(bitRead(data, i) ? 0x18 : 0x08);
	}
}

void sendFull(byte address, byte data) {
	sendBegin();
	sendByte(address);
	sendByte(~address);
	sendByte(data);
	sendByte(~data);
	sendEnd();
}

void setup() {
	sender.turnOn();
}

void loop() {
	static byte data = 0x00;
	sendFull(0x00, data);
	sendFull(0x00, data++);
	delay(200);
}

