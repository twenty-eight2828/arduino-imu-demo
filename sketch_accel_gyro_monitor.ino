#include <Wire.h>

#define MPU_ADDR 0x68           // MPU6050のI²Cアドレス（AD0がGNDのとき）
#define ACCEL_SCALE 16384.0     // ±2g設定時のスケーリング係数
#define GYRO_SCALE 131.0        // ±250dps設定時のスケーリング係数

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // MPU6050をスリープ解除（PWR_MGMT_1 = 0x00）
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0x00); // スリープ解除
  Wire.endTransmission();

  delay(100); // 初期化待機

  Serial.println("AccelX[g],AccelY[g],AccelZ[g],GyroX[dps],GyroY[dps],GyroZ[dps]");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // センサーデータの先頭アドレス
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14);

  if (Wire.available() == 14) {
    int16_t ax_raw = Wire.read() << 8 | Wire.read();
    int16_t ay_raw = Wire.read() << 8 | Wire.read();
    int16_t az_raw = Wire.read() << 8 | Wire.read();
    Wire.read(); Wire.read(); // 温度レジスタ（未使用）
    int16_t gx_raw = Wire.read() << 8 | Wire.read();
    int16_t gy_raw = Wire.read() << 8 | Wire.read();
    int16_t gz_raw = Wire.read() << 8 | Wire.read();

    // スケーリング処理
    float ax = ax_raw / ACCEL_SCALE;
    float ay = ay_raw / ACCEL_SCALE;
    float az = az_raw / ACCEL_SCALE;
    float gx = gx_raw / GYRO_SCALE;
    float gy = gy_raw / GYRO_SCALE;
    float gz = gz_raw / GYRO_SCALE;

    // CSV形式で出力
    Serial.print(ax, 2); Serial.print(",");
    Serial.print(ay, 2); Serial.print(",");
    Serial.print(az, 2); Serial.print(",");
    Serial.print(gx, 2); Serial.print(",");
    Serial.print(gy, 2); Serial.print(",");
    Serial.println(gz, 2);
  } else {
    Serial.println("データ取得失敗 ❌");
  }

  delay(500); // 0.5秒間隔で更新
}
