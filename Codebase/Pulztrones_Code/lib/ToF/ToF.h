#ifndef VL53L0X_MULTIPLEXER_H
#define VL53L0X_MULTIPLEXER_H

#include <Wire.h>
#include <VL53L0X.h>
#include "I2CMUX.h"

class VL53L0X_Multiplexer {
  public:
    // Constructor
    //VL53L0X_Multiplexer(uint8_t multiplexerAddress = 0x70);

    // Initialize the sensors
    void begin(I2CMUX &mux);

    // Function to get distance from the specific sensor
    uint16_t getTofDistance(uint8_t sensorChannel, I2CMUX &mux);

    float getAverageTofDistance(uint8_t sensorChannel, I2CMUX &mux);

  private:
    // Multiplexer address
    uint8_t tcaAddr;

    // VL53L0X sensor objects
    VL53L0X sensor1;
    VL53L0X sensor2;
    VL53L0X sensor3;

    // Function to select TCA9548A channel
    //void tcaSelect(uint8_t channel);
};

#endif // VL53L0X_MULTIPLEXER_H
