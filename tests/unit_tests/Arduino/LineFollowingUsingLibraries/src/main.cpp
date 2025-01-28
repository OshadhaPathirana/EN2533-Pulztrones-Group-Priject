/**
 * @file line_following_robot.ino
 * @brief Line following robot using QTR sensors, L298N motor driver, and OLED display
 * 
 * Pin Assignments:
 * @pin A0-A7 QTR sensor array inputs
 * @pin 26 QTR sensor array emitter pin
 * @pin 2 Left motor enable (ENA)
 * @pin 3 Right motor enable (ENB)
 * @pin 22 Left motor input 1 (IN1)
 * @pin 23 Left motor input 2 (IN2)
 * @pin 24 Right motor input 1 (IN3)
 * @pin 25 Right motor input 2 (IN4)
 * @pin 20 I2C SDA (implicitly used by Wire library for OLED)
 * @pin 21 I2C SCL (implicitly used by Wire library for OLED)
 * 
 * Other hardware connections:
 * - OLED Display: Connected via I2C (SDA and SCL)
 */

#include <Arduino.h>
#include <QTRSensors.h>
#include <PID_v1.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// QTR-A Sensor Array
#define NUM_SENSORS 8
#define EMITTER_PIN 26
QTRSensors qtr;
uint16_t sensorValues[NUM_SENSORS];

// Motor Driver Pins
#define ENA 2
#define IN1 22
#define IN2 23
#define IN3 24
#define IN4 25
#define ENB 3

// PID Control
double Setpoint, Input, Output;
double Kp = 2, Ki = 0, Kd = 1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Constants
#define MAX_SPEED 255
#define BASE_SPEED 150

void setMotorSpeeds(int, int);
void calibrateSensors();
void updateDisplay(uint16_t position, double output, int leftSpeed, int rightSpeed);

void setup() {
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Initializing...");
  display.display();

  // Configure the QTR sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, NUM_SENSORS);
  qtr.setEmitterPin(EMITTER_PIN);

  // Initialize motor control pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Calibrate the sensors
  calibrateSensors();

  // Initialize PID
  Setpoint = 3500; // Adjust based on your sensor array
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-MAX_SPEED, MAX_SPEED);
}

void calibrateSensors() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Calibrating...");
  display.display();
  
  // Turn on LED to indicate we are in calibration mode
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (uint16_t i = 0; i < 250; i++) {
    qtr.calibrate();
    delay(20);
  }
  
  // Turn off LED to indicate calibration is complete
  digitalWrite(LED_BUILTIN, LOW);
  
  display.println("Calibration complete");
  display.display();
  delay(1000);
}

void loop() {
  // Read sensor values
  uint16_t position = qtr.readLineBlack(sensorValues);

  // Compute PID
  Input = position;
  myPID.Compute();

  // Calculate motor speeds
  int leftSpeed = BASE_SPEED + Output;
  int rightSpeed = BASE_SPEED - Output;

  // Constrain speeds
  leftSpeed = constrain(leftSpeed, 0, MAX_SPEED);
  rightSpeed = constrain(rightSpeed, 0, MAX_SPEED);

  // Set motor directions and speeds
  setMotorSpeeds(leftSpeed, rightSpeed);

  // Update display with debug information
  updateDisplay(position, Output, leftSpeed, rightSpeed);

  delay(10);
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Left motor
  digitalWrite(IN1, leftSpeed > 0 ? HIGH : LOW);
  digitalWrite(IN2, leftSpeed > 0 ? LOW : HIGH);
  analogWrite(ENA, abs(leftSpeed));

  // Right motor
  digitalWrite(IN3, rightSpeed > 0 ? HIGH : LOW);
  digitalWrite(IN4, rightSpeed > 0 ? LOW : HIGH);
  analogWrite(ENB, abs(rightSpeed));
}

void updateDisplay(uint16_t position, double output, int leftSpeed, int rightSpeed) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Pos: ");
  display.println(position);
  display.print("Out: ");
  display.println(output);
  display.print("L: ");
  display.print(leftSpeed);
  display.print(" R: ");
  display.println(rightSpeed);
  display.display();
}