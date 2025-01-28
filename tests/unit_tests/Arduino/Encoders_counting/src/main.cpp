#include <Arduino.h>
#include "BluetoothDebug.h"

// put function declarations here:
int myFunction(int, int);
const int encoderA1 = 2; // Encoder 1 A pin
const int encoderB1 = 3; // Encoder 1 B pin
const int encoderA2 = 4; // Encoder 2 A pin
const int encoderB2 = 5; // Encoder 2 B pin

volatile int count1 = 0; // Count for encoder 1
volatile int count2 = 0; // Count for encoder 2



void setup() {
  // put your setup code here, to run once:
  initBluetoothDebug();
  pinMode(encoderA1, INPUT);
  pinMode(encoderB1, INPUT);
  pinMode(encoderA2, INPUT);
  pinMode(encoderB2, INPUT);
  
  int result = myFunction(2, 3);

  attachInterrupt(digitalPinToInterrupt(encoderA1), readEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderA2), readEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderB1), readEncoder2, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderB2), readEncoder2, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderA2), readEncoder2, RISING);
 
}

void loop() {
  // put your main code here, to run repeatedly:
   String msg1 = "Encoder 1 Count: " + String(count1);
    sendBluetoothData(msg1);  // Send encoder 1 count via Bluetooth

    String msg2 = "Encoder 2 Count: " + String(count2);
    sendBluetoothData(msg2);  // Send encoder 2 count via Bluetooth

    logToMonitor("Monitoring Encoder Counts...");  // Log a message to the monitor for debugging

    delay(100); // Update every 100 ms
}
void readEncoder1() { //take the values from the first encoder but not sure whether it is A1 and B1 or A1 and A2, so look here!!
    // Check the state of A
    int aState = digitalRead(encoderA1);
    // Check the state of B
    int bState = digitalRead(encoderB1);

    // Determine direction
    if (aState == HIGH) {
        if (bState == LOW) {
            count1++; // Clockwise rotation
        } else {
            count1--; // Counter-clockwise rotation
        }
    } else {
        if (bState == HIGH) {
            count1++; // Clockwise rotation
        } else {
            count1--; // Counter-clockwise rotation
        }
    }
}
void readEncoder2() {
    // Check the state of A
    int aState = digitalRead(encoderA2);
    // Check the state of B
    int bState = digitalRead(encoderB2);

    // Determine direction
    if (aState == HIGH) {
        if (bState == LOW) {
            count2++; // Clockwise rotation
        } else {
            count2--; // Counter-clockwise rotation
        }
    } else {
        if (bState == HIGH) {
            count2++; // Clockwise rotation
        } else {
            count2--; // Counter-clockwise rotation
        }
    }
}
// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

