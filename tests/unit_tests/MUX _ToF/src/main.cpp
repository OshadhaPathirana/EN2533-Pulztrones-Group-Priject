#include <Wire.h>
#include <VL53L0X.h>

#define PCA9548A_ADDRESS 0x70  // I2C address for PCA9548A

VL53L0X vl53[3];  // Array for 3 VL53L0X sensors

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Starting VL53L0X sensors with PCA9548A multiplexer");

  // Initialize each sensor on a different channel
  for (uint8_t i = 2; i < 3; i++) {
    selectChannel(i);
    if (!vl53[i].init()) {
      Serial.print(F("Failed to initialize sensor "));
      Serial.println(i);
      while (1);  // Halt if sensor initialization fails
    }
    vl53[i].setTimeout(500);
    Serial.print(F("Sensor "));
    Serial.print(i);
    Serial.println(F(" initialized"));
  }
}

void loop() {
  for (uint8_t i = 0; i < 3; i++) {
    selectChannel(i);
    uint16_t distance = vl53[i].readRangeSingleMillimeters();

    if (!vl53[i].timeoutOccurred()) {  // Check for a valid range reading
      Serial.print(F("Sensor "));
      Serial.print(i);
      Serial.print(F(": "));
      Serial.print(distance);
      Serial.println(F(" mm"));
    } else {
      Serial.print(F("Sensor "));
      Serial.print(i);
      Serial.println(F(" out of range"));
    }
    delay(100);  // Small delay between readings
  }

  delay(500);  // Delay between sets of readings for all sensors
}

void selectChannel(uint8_t channel) {
  if (channel > 7) return;  // Only 8 channels (0-7) on PCA9548A
  Wire.beginTransmission(PCA9548A_ADDRESS);
  Wire.write(1 << channel);  // Activate the selected channel
  Wire.endTransmission();
}
