#include "drv_hd44780_i2c.h"
// lib drv_hd44780_i2c host on https://github.com/aguegu/dot-matrix/tree/master/drv_hd44780_i2c
// which also needs lib twi host on https://github.com/aguegu/Arduino/tree/master/libraries/Twi

#include "Ds1302.h"

DrvHd44780I2c lcd(0x20 + 7);
Ds1302 rtc(5, 6, 7);

// CSK on pin #5
// I/O on pin #6
// RST on pin #7

#define SERIAL_CACHE_LENGTH 21
char gserial_cache[SERIAL_CACHE_LENGTH];

void setTimeBySerial();

void setup()
{
	Serial.begin(9600);

	lcd.init();

	rtc.setWritable(true);
	rtc.setCharger();
	rtc.halt(false);
	rtc.setWritable(false);
}

void loop()
{
	if (gserial_cache[SERIAL_CACHE_LENGTH - 1] == ']')
		setTimeBySerial();

	rtc.refresh(false);
	lcd.printf("%02x/%02x/%02x     [%x]", rtc.getHexData(MONTH),
			rtc.getHexData(DAY_OF_MONTH), rtc.getHexData(YEAR),
			rtc.getHexData(DAY_OF_WEEK));
	lcd.printf(0x10, "%02x:%02x:%02x", rtc.getHexData(HOUR),
			rtc.getHexData(MINUTE), rtc.getHexData(SECOND));
	lcd.putCache();

	delay(200);
}

void setTimeBySerial()
{
	byte data[7];
	for (byte i = 0; i < 7; i++)
	{
		data[i] = atoi(gserial_cache + 1 + 3 * i);
	}
	rtc.setTime(data[0], data[1], data[2]); // hour, minute, second
	rtc.setDate(data[5], data[3], data[4], data[6]); // year, month, day_of_month, day_of_week

	memset(gserial_cache, 0, SERIAL_CACHE_LENGTH);

	char tmp[48];

	snprintf(tmp, 48, "DateTime set to %02d:%02d:%02d %02d/%02d/%02d [%d]",
			data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
	Serial.println(tmp);

}

void serialEvent()
{
	static byte index = 0;

	while (Serial.available())
	{
		byte data = Serial.read();
		if (index == 0)
		{
			gserial_cache[index] = data;
			if (data == '[')
				index++;
		}
		else
		{
			gserial_cache[index] = data;
			index++;
			if (index >= SERIAL_CACHE_LENGTH)
				index = 0;
		}
	}
}
