#ifndef COIN_DROPPER_H
#define COIN_DROPPER_H

#include <Arduino.h>
#include <Servo.h>
#include "config.h"

class CoinDropper {
public:
    // Constructor
    CoinDropper();

    // Initialization method
    void init(int dropperServoPin);

    // Coin dropping methods
    void dropCoin();
    void resetDropper();

private:
    Servo dropperServo;

    // Dropper Servo Positions
    static const int DROPPER_MIN_ANGLE = 140;
    static const int DROPPER_MAX_ANGLE = 180;
    static const int DROPPER_REST_ANGLE = 180;
    static const int DROPPER_DROP_ANGLE = 150;

    // Timing constants
    static const int MOVEMENT_PAUSE = 200;

    // Internal utility method
    void moveDropperServo(int angle);
};

#endif // COIN_DROPPER_H