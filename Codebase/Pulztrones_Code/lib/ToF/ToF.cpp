#include "ToF.h"


// VL53L0X_Multiplexer::VL53L0X_Multiplexer(uint8_t multiplexerAddress) {
//   tcaAddr = multiplexerAddress;
// }

// Initialize the sensors
void VL53L0X_Multiplexer::begin(I2CMUX &mux) {
  Wire.begin();

  // Initialize each sensor on the respective channel // need to find the positions of the sensors
  //tcaSelect(2);
  mux.selectChannel(2);
  sensor1.init();
  sensor1.setTimeout(250);

  mux.selectChannel(3);
  sensor2.init();
  sensor2.setTimeout(250);

  mux.selectChannel(4);
  sensor3.init();
  sensor3.setTimeout(250);

  Serial.println("VL53L0X Sensors Initialized");
}

// // Function to select TCA9548A channel
// void VL53L0X_Multiplexer::tcaSelect(uint8_t channel) {
//   if (channel > 7) return;
//   Wire.beginTransmission(tcaAddr);
//   Wire.write(1 << channel);
//   Wire.endTransmission();
// }

// Function to get distance from the specified sensor channel
uint16_t VL53L0X_Multiplexer::getTofDistance(uint8_t sensorChannel, I2CMUX &mux) {
  uint16_t distance = 0;

  switch (sensorChannel) {
    case 2:
      mux.selectChannel(2);
      //tcaSelect(2);
      distance = sensor1.readRangeSingleMillimeters();
      if (sensor1.timeoutOccurred()) {
        Serial.println("Sensor 1 timeout!");
        //down
      }
      break;

    case 3:
      mux.selectChannel(3);
      //tcaSelect(3);
      distance = sensor2.readRangeSingleMillimeters();
      if (sensor2.timeoutOccurred()) {
        Serial.println("Sensor 2 timeout!");
        //middle
      }
      break;

    case 4:
      mux.selectChannel(4);
      //tcaSelect(4);
      distance = sensor3.readRangeSingleMillimeters();
      if (sensor3.timeoutOccurred()) {
        Serial.println("Sensor 3 timeout!");
      }
      break;

    default:
      Serial.println("Invalid sensor channel");
      break;
  }

  return distance;
}


float VL53L0X_Multiplexer::getAverageTofDistance(uint8_t sensorChannel, I2CMUX &mux) {
  uint32_t totalDistance = 0; 
  uint8_t validReadings = 0;

  for (int i = 0; i < 10; i++) {
    int distance = getTofDistance(sensorChannel, mux);

    if (distance > 0) 
    { 
      totalDistance += distance;
      validReadings++;
    } 
    // else 
    // {
    //   Serial.println("Timeout");
    // }

    
  }
  return totalDistance / validReadings ; 
  
  }
  

  

