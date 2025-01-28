#include <Arduino.h>
#include "encoders.h"


#define LEFT_ENCODER_PIN_A 19   
#define LEFT_ENCODER_PIN_B 27   

#define RIGHT_ENCODER_PIN_A 18 
#define RIGHT_ENCODER_PIN_B 26 

// Encoder counts
volatile long leftEncoderCount = 0;
volatile long rightEncoderCount = 0;

// Interrupt Service Routine for left encoder channel A
void leftEncoderISR() {
    // Determine direction using channel B
    if (digitalRead(LEFT_ENCODER_PIN_B) == HIGH) {
        leftEncoderCount++;
    } else {
        leftEncoderCount--;
    }
}

// Interrupt Service Routine for right encoder channel A
void rightEncoderISR() {
    // Determine direction using channel B
    if (digitalRead(RIGHT_ENCODER_PIN_B) == HIGH) {
        rightEncoderCount--;
    } else {
        rightEncoderCount++;
    }
}

void initEncoders() {
    // Initialize encoder pins
    pinMode(LEFT_ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(LEFT_ENCODER_PIN_B, INPUT_PULLUP);
    
    pinMode(RIGHT_ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(RIGHT_ENCODER_PIN_B, INPUT_PULLUP);

    // Attach interrupts to encoder pins
    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN_A), leftEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN_A), rightEncoderISR, RISING);
}

long getLeftEncoderCounts() {
    return leftEncoderCount;
}

long getRightEncoderCounts() {
    return rightEncoderCount;
}

void resetEncoders() {
    leftEncoderCount = 0;
    rightEncoderCount = 0;
}