#ifndef I2CMUX_H
#define I2CMUX_H

#include <Wire.h>
#include <Arduino.h>

class I2CMUX {
private:
    uint8_t address; // Address of the TCA9548A

public:
    // Constructor to set the I2C address
    I2CMUX(uint8_t addr = 0x70);

    // Method to initialize the multiplexer
    void begin();

    // Select a specific channel (0-7)
    void selectChannel(uint8_t channel);

    // Disable all channels
    void disableAll();

    // Check if a specific channel is active (not supported by all implementations)
    bool isChannelActive(uint8_t channel);
};

#endif // I2CMUX_H
