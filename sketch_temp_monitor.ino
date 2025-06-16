#include <Wire.h>

#define TMP102_ADDR 0x48  // TMP102のアドレス（AD0 = GND）
#define TEMP_REG    0x00  // 温度レジスタ

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("TMP102 initialized (raw access).");
}

void loop() {
  float tempC = readTMP102();

  Serial.print("Temperature (°C): ");
  Serial.println(tempC, 2);

  delay(1000);
}

float readTMP102() {
  Wire.beginTransmission(TMP102_ADDR);
  Wire.write(TEMP_REG);
  if (Wire.endTransmission() != 0) {
    Serial.println("TMP102 not responding ❌");
    return -999.0;
  }

  Wire.requestFrom(TMP102_ADDR, 2);
  if (Wire.available() < 2) {
    Serial.println("TMP102 data not available ❌");
    return -999.0;
  }

  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();

  int16_t raw = (msb << 8 | lsb) >> 4;
  if (raw & 0x800) raw |= 0xF000;

  return raw * 0.0625;
}
