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

#include <Hx711.h>

Hx711 scale(A1, A0);

void setup() {

  Serial.begin(9600);

}

void loop() {

  Serial.print(scale.getGram(), 1);
  Serial.println(" g");

  delay(200);
}

