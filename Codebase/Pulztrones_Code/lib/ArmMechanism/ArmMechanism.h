#ifndef ARM_MECHANISM_H
#define ARM_MECHANISM_H

#include <Arduino.h>
#include <Servo.h>
#include "config.h"

class ArmMechanism {
public:
    // Constructor
    ArmMechanism();

    // Initialization method
    void init(int armLiftServoPin, int gripperServoPin);

    // Arm movement methods
    void moveToPickupPosition();
    void moveToRestPosition();
    void openGripper();
    void closeGripper();
    void holdBox();

    void moveto_TOF_Reading_Position();
    void moveToRestPosition_without_off();


private:
    Servo armLiftServo;
    Servo gripperServo;

    // Arm Lift Servo Positions
    static const int ARM_LIFT_MIN_ANGLE = 75;
    static const int ARM_LIFT_MAX_ANGLE = 111;
    static const int ARM_REST_ANGLE = 111;
    static const int ARM_PICKUP_ANGLE = 75;
    static const int ARM_GATE_DETECTION_ANGLE = 105;

    // Gripper Servo Positions
    static const int GRIPPER_MIN_ANGLE = 0;
    static const int GRIPPER_MAX_ANGLE = 120;
    static const int GRIPPER_OPEN_ANGLE = 0;
    static const int GRIPPER_CLOSE_ANGLE = 110;
    static const int GRIPPER_HOLD_ANGLE = 120;//115

    // Timing constants
    static const int MOVEMENT_PAUSE = 500;

    // Internal utility methods
    void moveArmLiftServo(int angle);
    void moveGripperServo(int angle);
    
};

#endif // ARM_MECHANISM_H