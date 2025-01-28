#include "BluetoothDebug.h"
#include "encoders.h"
#include "irs.h"


// Initialize Bluetooth communication on Serial2
void initBluetoothDebug(long baudRate) {
    Serial2.begin(baudRate);  // Initialize Bluetooth at the specified baud rate
    delay(1000);              // Give time for everything to initialize
    Serial.println("Bluetooth Debugging Ready!");
}


// Function to send data via Bluetooth
void sendBluetoothData(String data) {
    Serial2.print(data);
}

void sendBluetoothDataln(String data) {
    Serial2.println(data);
}

void printEncoderData() {
    static unsigned long lastPrintTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastPrintTime >= 100) {
        Serial2.print("Left Encoder: ");
        Serial2.print(getLeftEncoderCounts());
        Serial2.print(", Right Encoder: ");
        Serial2.println(getRightEncoderCounts());
        lastPrintTime = currentTime;
    }
}

void printIRData() {
    // Read QTR sensor values and get the line position
    int16_t position = readWhiteLinePosition();

    // Print QTR sensor values to Serial2
    for (uint8_t i = 0; i < SensorCount; i++) {
        Serial2.print(sensorValues[i]);
        if (i < SensorCount - 1) {
            Serial2.print(", ");
        }
    }
    Serial2.println();

    delay(100);
}