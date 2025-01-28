#include "I2CMUX.h"

// Constructor
I2CMUX::I2CMUX(uint8_t addr) : address(addr) {}

// Initialize the multiplexer
void I2CMUX::begin() {
    Wire.begin();
}

// Select a specific channel (0-7)
void I2CMUX::selectChannel(uint8_t channel) {
    if (channel > 7) return; // Valid channels: 0 to 7
    Wire.beginTransmission(address);
    Wire.write(1 << channel);
    Wire.endTransmission();
    delay(100); // Wait for the channel to be selected
}

// Disable all channels
void I2CMUX::disableAll() {
    Wire.beginTransmission(address);
    Wire.write(0x00);
    Wire.endTransmission();
}

// Check if a specific channel is active (not supported by all implementations)
bool I2CMUX::isChannelActive(uint8_t channel) {
    if (channel > 7) return false;
    Wire.beginTransmission(address);
    uint8_t status = Wire.endTransmission();
    return (status == 0);
}
