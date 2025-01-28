#include "Ultrasonic.h"

// Constructor
Ultrasonic::Ultrasonic(int trigPin, int echoPin) {
    _trigPin = trigPin;
    _echoPin = echoPin;
    pinMode(_trigPin, OUTPUT);  // Set the trigger pin as output
    pinMode(_echoPin, INPUT);   // Set the echo pin as input
}

// Measure a single distance using the ultrasonic sensor
float Ultrasonic::getDistance() {
    digitalWrite(_trigPin, LOW);        // Send a low pulse to the trigger pin
    delayMicroseconds(2);               // Wait for 2 microseconds
    digitalWrite(_trigPin, HIGH);       // Send a high pulse to the trigger pin
    delayMicroseconds(10);              // Wait for 10 microseconds
    digitalWrite(_trigPin, LOW);        // End the pulse on the trigger pin

    float duration = pulseIn(_echoPin, HIGH);  // Measure the duration of the echo pulse
    return (duration * 0.0343) / 2;           // Convert the duration to distance in cm
}

// Measure multiple distances and store them in an array
void Ultrasonic::measureDistances(float arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = getDistance();      // Store each measured distance in the array
        delay(100);                  // Delay between measurements to avoid interference
    }
}

// Calculate the average of an array of distances and return 1 if the average is less than 25, else return 0
int Ultrasonic::BlockedOrNot(float arr[], int size) {
    float sum = 0;

    // Sum all values in the array
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }

    // Calculate the average
    float average = sum / size;

    // Return 1 if the average is less than 25, else return 0
    return (average < 25.0) ? 1 : 0;
}