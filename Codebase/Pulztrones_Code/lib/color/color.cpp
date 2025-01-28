#include "Color.h"

// Pin definitions for the TCS230 sensor
const int leftS0 = 38, leftS1 = 42, leftS2 = 41, leftS3 = 40, leftSensorOut = 39;

// Adjustable thresholds for red and blue detection

int redThreshold[3][2] = {{25, 200}, {200, 500}, {50, 195}};  // {Rmin, Rmax}, {Gmin, Gmax}, {Bmin, Bmax}
int blueThreshold[3][2] = {{200, 600}, {80, 600}, {30, 130}}; // {Rmin, Rmax}, {Gmin, Gmax}, {Bmin, Bmax}


// Initialize the sensor
void setupSensor(int S0, int S1, int S2, int S3, int sensorOut) {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);

    // Set frequency scaling to 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
}

// Read raw RGB values from the sensor
void readRGB(int S0, int S1, int S2, int S3, int sensorOut, int &red, int &green, int &blue) {
    // Reading Red Frequency
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red = pulseIn(sensorOut, LOW);
    delay(100);

    // Reading Green Frequency
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    green = pulseIn(sensorOut, LOW);
    delay(100);

    // Reading Blue Frequency
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blue = pulseIn(sensorOut, LOW);
    delay(100);
}

// Detect color based on thresholds
String detectColor(int red, int green, int blue) {
    // Check for red
    if (red >= redThreshold[0][0] && red <= redThreshold[0][1] &&
        green >= redThreshold[1][0] && green <= redThreshold[1][1] &&
        blue >= redThreshold[2][0] && blue <= redThreshold[2][1]) {
        return "RED";
    }
    // Check for blue
    else if (red >= blueThreshold[0][0] && red <= blueThreshold[0][1] &&
             green >= blueThreshold[1][0] && green <= blueThreshold[1][1] &&
             blue >= blueThreshold[2][0] && blue <= blueThreshold[2][1]) {
        return "BLUE";
    }

    return "UNKNOWN";

}
