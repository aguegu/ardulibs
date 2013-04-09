// Tile.ino
// Created on: Sep 15, 2012
// Author: Weihong Guan (aGuegu)
// Blog:  http://aguegu.net
// Email: weihong.guan@gmail.com
// Host on: https://github.com/aguegu/Arduino
// Adept Kalman Filter to the readings of Accelerometer ADXL345 and Gyroscope ITG3200
// run Tile.pde with Processing to check the data from Serial

#include "Wire.h"
#include "KalmanFilter.h"

void writeToWire(int device, byte address, byte val);
void readFromWire(int device, byte address, int count, byte * p);
void sendFloat(float val);


KalmanFilter kf(0.004);	// dt = 4ms = 0.004s

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Wire.begin();

  // initial ADXL345
  writeToWire(0x53, 0x2d, 0x08);
  writeToWire(0x53, 0x2c, 0x0a);
  writeToWire(0x53, 0x31, 0x08);

  // initial ITG3200
  writeToWire(0x68, 0x3e, 0x00);
  writeToWire(0x68, 0x15, 0x09);
  writeToWire(0x68, 0x16, 0x1e);
  writeToWire(0x68, 0x17, 0x00);
}

void loop()
{
  uint32_t now = millis();
  uint32_t due_to = now + 4;	// how long a single loop lasts? let's say 4 ms

  byte data[6];

  int acc[3];
  int angular_rate[3];

  readFromWire(0x53, 0x32, 6, data);
  for (byte i = 0; i < 3; i++)
  {
    acc[i] = makeWord(data[i + i + 1], data[i + i]);
  }

  readFromWire(0x68, 0x1d, 6, data);
  for (byte i = 0; i < 3; i++)
  {
    angular_rate[i] = makeWord(data[i + i], data[i + i + 1]);
  }

  kf.kalman_update(acc[1], acc[2]);
  kf.state_update((float) angular_rate[0] * DEG_TO_RAD / 14.375);

  float angle_acc = atan2(-acc[2], acc[1]);
  float angle_kalman = kf.getAngle();

  sendFloat(angle_acc);
  sendFloat(angle_kalman);

  Serial.write(0xff);

  while (millis() < due_to)
    digitalWrite(LED_BUILTIN, HIGH);

  digitalWrite(LED_BUILTIN, LOW);
}

void sendFloat(float val)
{
  byte * p;
  p = (byte *) &val;
  Serial.Print::write(p, sizeof(float));
}

void writeToWire(int device, byte address, byte val)
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}

void readFromWire(int device, byte address, int count, byte * p)
{
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(device, count);

  while (Wire.available())
  {
    *(p++) = Wire.read();
  }
}



