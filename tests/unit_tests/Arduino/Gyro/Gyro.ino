#include <Wire.h>

#define MPU6500_ADDR 0x68
#define GYRO_CONFIG 0x1B
#define GYRO_XOUT_H 0x43

void setup() {
  Wire.begin();
  Serial2.begin(9600);
  
  // Initialize MPU-6500
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up the MPU-6500
  Wire.endTransmission(true);
  
  // Configure gyroscope range
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x08);  // Set full scale range to ±500 degrees/second
  Wire.endTransmission(true);
}

void loop() {
  int16_t gyroX, gyroY, gyroZ;
  
  // Read gyroscope data
  Wire.beginTransmission(MPU6500_ADDR);
  Wire.write(GYRO_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6500_ADDR, 6, true);
  
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();
  
  // Convert raw values to degrees per second
  float gyroX_dps = gyroX / 65.5;
  float gyroY_dps = gyroY / 65.5;
  float gyroZ_dps = gyroZ / 65.5;
  
  // Print gyroscope values
  Serial2.print("Gyro X: ");
  Serial2.print(gyroX_dps);
  Serial2.print(" deg/s, Gyro Y: ");
  Serial2.print(gyroY_dps);
  Serial2.print(" deg/s, Gyro Z: ");
  Serial2.print(gyroZ_dps);
  Serial2.println(" deg/s");
  
  delay(100);  // Delay for readability
}