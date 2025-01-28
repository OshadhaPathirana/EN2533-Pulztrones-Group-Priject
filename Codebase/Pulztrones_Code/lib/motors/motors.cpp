#include <Arduino.h>
#include <motors.h>

// Initialize motor control pins
const int rightMotorBackward = 22;
const int rightMotorForward = 23;
const int leftMotorBackward = 25;
const int leftMotorForward = 24;

const int enaPin = 5;  // Left motor PWM pin
const int enbPin = 4;  // Right motor PWM pin


/*
 * Function to initialize motor control pins.
 * Sets all motor control and PWM pins as OUTPUT.
 */
void initMotorPins() {
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);

  pinMode(enaPin, OUTPUT);
  pinMode(enbPin, OUTPUT);
}


/*
 * Set speed and direction for the left motor.
 * Speed is between -255 to 255. Positive for forward, negative for backward.
 */
void setMotorLPWM(int speed) {
  if (speed >= 0) {
    analogWrite(enaPin, speed); // Set PWM for left motor forward direction
    digitalWrite(leftMotorForward, HIGH);
    digitalWrite(leftMotorBackward, LOW);
  } else {
    analogWrite(enaPin, -speed); // Set PWM for left motor backward direction
    digitalWrite(leftMotorForward, LOW);
    digitalWrite(leftMotorBackward, HIGH);
  }
}

/*
 * Set speed and direction for the right motor.
 * Speed is between -255 to 255. Positive for forward, negative for backward.
 */
void setMotorRPWM(int speed) {
  if (speed >= 0) {
    analogWrite(enbPin, speed); // Set PWM for right motor forward direction
    digitalWrite(rightMotorForward, HIGH);
    digitalWrite(rightMotorBackward, LOW);
  } else {
    analogWrite(enbPin, -speed); // Set PWM for right motor backward direction
    digitalWrite(rightMotorForward, LOW);
    digitalWrite(rightMotorBackward, HIGH);
  }
}

/*
 * Stop both motors by setting PWM to 0 and all control pins to LOW.
 */
void resetMotors() {
  analogWrite(enaPin, 0); // Set PWM for left motor to 0
  analogWrite(enbPin, 0); // Set PWM for right motor to 0
  
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

/*
 * Function to move both motors forward with given speeds.
 */
void moveForward(int SpeedM1, int SpeedM2) {
  setMotorLPWM(SpeedM1);
  setMotorRPWM(SpeedM2);
}

void MotorBreak() {
    // Set both control pins to HIGH for braking
    digitalWrite(leftMotorForward, HIGH);
    digitalWrite(leftMotorBackward, HIGH);
    digitalWrite(rightMotorForward, HIGH);
    digitalWrite(rightMotorBackward, HIGH);
    
    delay(200);  // Delay to allow motors to stop

    resetMotors();
}

