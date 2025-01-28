#ifndef PID_H
#define PID_H

// Constants for PID control
extern const float KP;        // Proportional gain
extern const float KD;        // Derivative gain

// Function declarations
float PIDLine(int error);
float PIDLineReverse(int error);
float PIDEnc(int error);


#endif // PID_H