#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "Arduino.h"

class Ultrasonic {
public:
    // Constructor to initialize the trigger and echo pins
    Ultrasonic(int trigPin, int echoPin);

    // Measure a single distance using the ultrasonic sensor
    float getDistance();

    // Measure multiple distances and store them in an array
    void measureDistances(float arr[], int size);
    
    // Calculate the average of an array of distances and return 1 if it's less than 25, else return 0
    int BlockedOrNot(float arr[], int size);

private:
    int _trigPin; // Trigger pin for the ultrasonic sensor
    int _echoPin; // Echo pin for the ultrasonic sensor
};

#endif