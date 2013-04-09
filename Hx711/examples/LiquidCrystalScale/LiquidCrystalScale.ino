/* sample for digital weight scale of hx711, display with a HD44780 liquid crtstal monitor
 *
 * hardware design: syyyd
 * available at http://syyyd.taobao.com
 *
 * library design: Weihong Guan (@aguegu)
 * http://aguegu.net
 *
 * library host on
 * https://github.com/aguegu/Arduino
 */

// Hx711.DOUT - pin #A1
// Hx711.SCK - pin #A0

// LCD.RS - pin 12
// LCD.En - pin 11
// LCD.D4 - pin 5
// LCD.D5 - pin 4
// LCD.D6 - pin 3
// LCD.D7 - pin 2

#include <LiquidCrystal.h>
#include <Hx711.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Hx711 scale(A1, A0);

void setup() {

  lcd.begin(16, 2);

}

void loop() {

  lcd.setCursor(0, 0);
  lcd.print(scale.getGram(), 1);
  lcd.print(" g");
  lcd.print("       ");

  delay(200);
}

