#include <Wire.h>

#define MPU_ADDR    0x68
#define TMP102_ADDR 0x48

const float ACCEL_SCALE = 16384.0; // ±2g
const float GYRO_SCALE  = 131.0;   // ±250°/s

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // MPU6050 スリープ解除
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0x00); // スリープ解除
  Wire.endTransmission();

  delay(100);

  // CSVヘッダー出力
  Serial.println("Temp_C,Accel_X[g],Accel_Y[g],Accel_Z[g],Gyro_X[dps],Gyro_Y[dps],Gyro_Z[dps]");
}

void loop() {
  // TMP102から温度を取得
  float tempC = readTMP102();

  // MPU6050から加速度・ジャイロを取得
  int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
  readMPU6050(ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw);

  // スケーリング
  float ax = ax_raw / ACCEL_SCALE;
  float ay = ay_raw / ACCEL_SCALE;
  float az = az_raw / ACCEL_SCALE;
  float gx = gx_raw / GYRO_SCALE;
  float gy = gy_raw / GYRO_SCALE;
  float gz = gz_raw / GYRO_SCALE;

  // CSV出力
  Serial.print(tempC, 2); Serial.print(",");
  Serial.print(ax, 2); Serial.print(",");
  Serial.print(ay, 2); Serial.print(",");
  Serial.print(az, 2); Serial.print(",");
  Serial.print(gx, 2); Serial.print(",");
  Serial.print(gy, 2); Serial.print(",");
  Serial.println(gz, 2);

  delay(500);
}

float readTMP102() {
  Wire.beginTransmission(TMP102_ADDR);
  Wire.write(0x00); // 温度レジスタ
  Wire.endTransmission();
  Wire.requestFrom(TMP102_ADDR, 2);

  if (Wire.available() == 2) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    int16_t rawTemp = ((msb << 8) | lsb) >> 4;
    if (rawTemp & 0x800) rawTemp |= 0xF000; // 負数補正
    return rawTemp * 0.0625;
  }

  return -999.0; // エラー時
}

void readMPU6050(int16_t &ax, int16_t &ay, int16_t &az, int16_t &gx, int16_t &gy, int16_t &gz) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // Accel_X_H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);

  if (Wire.available() >= 14) {
    ax = Wire.read() << 8 | Wire.read();
    ay = Wire.read() << 8 | Wire.read();
    az = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read(); // Temp（未使用）
    gx = Wire.read() << 8 | Wire.read();
    gy = Wire.read() << 8 | Wire.read();
    gz = Wire.read() << 8 | Wire.read();
  } else {
    ax = ay = az = gx = gy = gz = 0;
  }
}
