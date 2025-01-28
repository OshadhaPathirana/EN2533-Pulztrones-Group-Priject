#include "CoinDropper.h"

// Constructor
CoinDropper::CoinDropper() {
    // Constructor can be empty as we'll use init() for setup
}

// Initialization method
void CoinDropper::init(int dropperServoPin) {
    // Attach servo to the specified pin
    dropperServo.attach(dropperServoPin);
    digitalWrite(COIN_DROPPER_ON_PIN, HIGH);
    delay(200);
    // Set initial position
    moveDropperServo(DROPPER_REST_ANGLE);

    digitalWrite(COIN_DROPPER_ON_PIN, LOW);
    delay(200);
}

// Servo movement method with angle constraint
void CoinDropper::moveDropperServo(int angle) {
    int constrainedAngle = constrain(angle, DROPPER_MIN_ANGLE, DROPPER_MAX_ANGLE);
    dropperServo.write(constrainedAngle);
    delay(MOVEMENT_PAUSE);
}

// Drop coin sequence
void CoinDropper::dropCoin() {
    digitalWrite(COIN_DROPPER_ON_PIN, HIGH);
    delay(200);
    
    // Activate dropper
    moveDropperServo(DROPPER_DROP_ANGLE);
    delay(200);
    // Reset dropper
    moveDropperServo(DROPPER_REST_ANGLE);
    
    digitalWrite(COIN_DROPPER_ON_PIN, LOW);
    delay(200);
}

// Reset dropper to rest position
void CoinDropper::resetDropper() {
    moveDropperServo(DROPPER_REST_ANGLE);
}