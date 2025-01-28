#include <Arduino.h>
#include <BluetoothDebug.h>
#include <Adafruit_VL53L0X.h>
#include <QTRSensors.h>

// Define motor control pins
const int leftMotorForward = 23;
const int leftMotorBackward = 24;
const int rightMotorForward = 25;
const int rightMotorBackward = 26;
const int enaPin = 2; //right
const int enbPin = 3; //left

// Class instances
QTRSensors qtr;

// initializing the global variables
const uint16_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// Function to send data via Bluetooth
void sendBluetoothData(String data) {
  Serial2.println(data);  // Sending data via Serial2 for Bluetooth
}

void setup() {
  // Initialize Bluetooth debugging
  initBluetoothDebug();

  // Initialize sensor pins
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(2);

  // Start Serial2 for Bluetooth communication
  Serial2.begin(9600);  // Adjust the baud rate as per your Bluetooth module's settings
}

void loop() {
  // Read raw sensor values
  qtr.read(sensorValues);

  // Create a string to store the sensor values
  String sensorData = "";

  // Add each sensor value to the string
  for (uint8_t i = 0; i < SensorCount; i++) {
    sensorData += String(sensorValues[i]);
    if (i < SensorCount - 1) {
      sensorData += ',';  // Add comma between sensor values
    }
  }

  // Send the sensor values via Bluetooth
  sendBluetoothData(sensorData);

  // Add a small delay between readings
  delay(250);
}
