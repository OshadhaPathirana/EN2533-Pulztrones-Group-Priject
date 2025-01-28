#ifndef MOTORS_H_
#define MOTORS_H_

// Define motor control pins
extern const int rightMotorBackward;
extern const int rightMotorForward;
extern const int leftMotorBackward;
extern const int leftMotorForward;

extern const int enaPin; // Left motor PWM pin
extern const int enbPin; // Right motor PWM pin


void setMotorLPWM(int speed);
void setMotorRPWM(int speed);
void resetMotors();
void MotorBreak();

void moveForward(int SpeedM1, int SpeedM2);

// Function to initialize motor control pins
void initMotorPins();

#endif /* MOTORS_H_ */
