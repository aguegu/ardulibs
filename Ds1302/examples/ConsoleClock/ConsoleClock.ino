#include "Ds1302.h"

Ds1302 rtc(5, 6, 7);
// CSK on pin #5
// I/O on pin #6
// RST on pin #7

void setup()
{
	rtc.setWritable(true);
	rtc.setCharger();
	rtc.halt(false);
	rtc.setWritable(false);

	rtc.setDate(13, 1, 29, 2);
	rtc.setTime(23, 17, 00);

	Serial.begin(9600);
}

void loop()
{
	rtc.refresh(false);

	Serial.print(rtc.getDecData(MONTH));
	Serial.print("/");
	Serial.print(rtc.getDecData(DAY_OF_MONTH));
	Serial.print("/");
	Serial.print(rtc.getDecData(YEAR));

	Serial.print(" [");
	Serial.print(rtc.getDecData(DAY_OF_WEEK));
	Serial.print("] ");

	Serial.print(rtc.getDecData(HOUR));
	Serial.print(":");
	Serial.print(rtc.getDecData(MINUTE));
	Serial.print(":");
	Serial.print(rtc.getDecData(SECOND));

	Serial.println();

	delay(1000);
}
