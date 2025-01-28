#include <Arduino.h>
#include <QTRSensors.h>

// Define motor control pins
const int leftMotorForward = 22;
const int leftMotorBackward = 23;
const int rightMotorForward = 24;
const int rightMotorBackward = 25;
const int enaPin = 2; //right
const int enbPin = 3; //left

// Class instances
QTRSensors qtr;


void moveForward(int16_t SpeedM1, int16_t SpeedM2);

// initializing the global variables
const uint16_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

float lastError = 0;



// PID control variables
float KP = 1; // Proportional gain
//float KI = 0.0; // Integral gain
float KD = 1; // Derivative gain

// Motor Base speeds
int16_t M1 = 150;
int16_t M2 = 150;

// put function declarations here:
void calibrateIRSensors();

void setup() {
  // put your setup code here, to run once:
  calibrateIRSensors();
}

void loop() {
  int16_t position = qtr.readLineBlack(sensorValues);

  int16_t error = position - 3500;

  int16_t motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;

  int16_t m1Speed = M1 + motorSpeed;
  int16_t m2Speed = M2 - motorSpeed;


  // Ensure the motor speeds are within valid range
  m1Speed = constrain(m1Speed, 0, 255);
  m2Speed = constrain(m2Speed, 0, 255);

  moveForward(m1Speed, m2Speed);

}

// put function definitions here:
void calibrateIRSensors() {
  /**
 * @brief Calibrates the IR sensors for line following.
 * 
 * This function sets up and calibrates the QTR (Quadrature Track Recognition) 
 * IR sensors for optimal performance in line following applications.
 * 
 * The function performs the following steps:
 * 1. Sets the sensor type to analog
 * 2. Configures the sensor pins (A0 to A7)
 * 3. Sets the emitter pin to digital pin 22
 * 4. Runs a calibration routine for about 5 seconds
 * 
 * During the calibration phase, the sensors should be moved over both 
 * reflectance extremes they will encounter in the application.
 * 
 * @note The calibration process takes approximately 5 seconds 
 *       (250 iterations * 20 ms per iteration).
 * 
 * @pre The QTR object (qtr) and SensorCount constant should be defined 
 *      before calling this function.
 * 
 * @post After calling this function, the IR sensors will be calibrated 
 *       and ready for use in line following applications.
 */
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(22); // emitter is controlled by digital pin 22


  for (uint8_t i = 0; i < 250; i++)
  {
    qtr.calibrate();
    delay(20);
  }
} 

void moveForward(int16_t SpeedM1, int16_t SpeedM2) {
  /**
 * @brief Moves the robot forward at the specified speeds.
 * 
 * This function moves the robot forward at the specified speeds.
 * 
 * The function performs the following steps:
 * 1. Sets the motor speeds for the left and right motors
 * 2. Moves the robot forward
 * 
 * @param SpeedM1 The speed of the left motor (0 to 255)
 * @param SpeedM2 The speed of the right motor (0 to 255)
 * 
 * @pre The motor control pins should be defined before calling this function.
 * 
 * @post After calling this function, the robot will move forward at the 
 *       specified speeds.
 */
  analogWrite(enaPin, SpeedM1);
  analogWrite(enbPin, SpeedM2);

  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}