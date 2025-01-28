#include "ArmMechanism.h"

// Constructor
ArmMechanism::ArmMechanism() {
    // Constructor can be empty as we'll use init() for setup
}

// Initialization method
void ArmMechanism::init(int armLiftServoPin, int gripperServoPin) {
    // Attach servos to their respective pins
    armLiftServo.attach(armLiftServoPin);
    gripperServo.attach(gripperServoPin);

    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Set initial positions
    moveArmLiftServo(ARM_REST_ANGLE);
    moveGripperServo(GRIPPER_OPEN_ANGLE);

    digitalWrite(ARM_GRIPPER_ON_PIN, LOW);
    delay(200);

}

// Servo movement methods with angle constraint
void ArmMechanism::moveArmLiftServo(int angle) {
    int constrainedAngle = constrain(angle, ARM_LIFT_MIN_ANGLE, ARM_LIFT_MAX_ANGLE);
    armLiftServo.write(constrainedAngle);
    delay(MOVEMENT_PAUSE);
}

void ArmMechanism::moveGripperServo(int angle) {
    int constrainedAngle = constrain(angle, GRIPPER_MIN_ANGLE, GRIPPER_MAX_ANGLE);
    gripperServo.write(constrainedAngle);
    delay(MOVEMENT_PAUSE);
}

void ArmMechanism::moveToPickupPosition() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to pickup position
    int currentAngle = armLiftServo.read();
    int targetAngle = ARM_PICKUP_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        armLiftServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(12000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }
    // Ensure we end exactly at the target angle
    moveArmLiftServo(targetAngle);
}

void ArmMechanism::moveToRestPosition() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to rest position
    int currentAngle = armLiftServo.read();
    int targetAngle = ARM_REST_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        armLiftServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(12000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }
    // Ensure we end exactly at the target angle
    moveArmLiftServo(targetAngle);

    digitalWrite(ARM_GRIPPER_ON_PIN, LOW);
    delay(200);
}

void ArmMechanism::moveToRestPosition_without_off() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to rest position
    int currentAngle = armLiftServo.read();
    int targetAngle = ARM_REST_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        armLiftServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(12000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }
    // Ensure we end exactly at the target angle
    moveArmLiftServo(targetAngle);

}


void ArmMechanism::openGripper() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to open position
    int currentAngle = gripperServo.read();
    int targetAngle = GRIPPER_OPEN_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        gripperServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(6000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }

    moveGripperServo(targetAngle);

    digitalWrite(ARM_GRIPPER_ON_PIN, LOW);
    delay(200);
}

void ArmMechanism::closeGripper() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to close position
    int currentAngle = gripperServo.read();
    int targetAngle = GRIPPER_CLOSE_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        gripperServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(6000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }
    moveGripperServo(targetAngle);
}

void ArmMechanism::holdBox() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to hold position
    int currentAngle = gripperServo.read();
    int targetAngle = GRIPPER_HOLD_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        gripperServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(6000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }
    moveGripperServo(targetAngle);
}


void ArmMechanism::moveto_TOF_Reading_Position() {
    digitalWrite(ARM_GRIPPER_ON_PIN, HIGH);
    delay(200);

    // Gradually move from current position to pickup position
    int currentAngle = armLiftServo.read();
    int targetAngle = ARM_GATE_DETECTION_ANGLE;
    
    // Determine increment direction and magnitude
    int increment = (currentAngle < targetAngle) ? 1 : -1;
    
    // Continuous movement
    while (currentAngle != targetAngle) {
        // Move servo without additional delay
        armLiftServo.write(currentAngle);
        
        // Small micros delay for smooth movement
        delayMicroseconds(12000); // Adjust this value for speed control
        
        // Update angle
        currentAngle += increment;
        
        // Ensure we stop exactly at target
        if ((increment > 0 && currentAngle >= targetAngle) || 
            (increment < 0 && currentAngle <= targetAngle)) {
            currentAngle = targetAngle;
        }
    }
    // Ensure we end exactly at the target angle
    moveArmLiftServo(targetAngle);
}