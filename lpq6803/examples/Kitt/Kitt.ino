#include "lpd6803.h"

Lpd6803 lpd(12, 13, 5);
// DIN - #12
// SCK - #13
// 5 pcs of lpd6803 in serial

byte lason[9] =
{ 0x00, 0x00, 0x04, 0x10, 0x1f, 0x10, 0x04, 0x00, 0x00 };

void setup()
{

	lpd.setColor(0, 0x1f, 0, 0);
	lpd.setColor(1, 0x10, 0, 0);
	lpd.setColor(2, 0x04, 0, 0);
	lpd.setColor(3, 0x01, 0, 0);
	lpd.setColor(4, 0x00, 0, 0);

	lpd.display();

}

void loop()
{
	byte i = 0;
	for (; i < 4; i++)
	{
		for (byte j = 0; j < 5; j++)
		{
			lpd.setColor(j, lason[i + j], lason[i + j], 0);
		}
		lpd.display();

		delay(100);
	}

	for (; i > 0; i--)
	{
		for (byte j = 0; j < 5; j++)
		{
			lpd.setColor(j, lason[i + j], lason[i + j], 0);
		}
		lpd.display();

		delay(100);
	}
}
