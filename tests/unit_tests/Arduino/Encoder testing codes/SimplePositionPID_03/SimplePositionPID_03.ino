// Pins
const int encoderA[] = {2, 3}; // Encoder A pins
const int encoderB[] = {4, 5}; // Encoder B pins
const int pwmPins[] = {12, 13}; // PWM pins for motor control
const int motorIn1[] = {8, 10};//{8, 9}; // Motor direction pins 1
const int motorIn2[] = {9, 11};//{10, 11}; // Motor direction pins 2

// Globals
volatile int posi[] = {0, 0}; // Motor positions
const int targetPosition = 5000; // Target position for both motors
const int baseSpeed = 200; // Base speed for both motors
const int maxSpeed = 255; // Maximum speed limit

void setup() {
    Serial.begin(9600);

    for (int k = 0; k < 2; k++) {
        pinMode(encoderA[k], INPUT);
        pinMode(encoderB[k], INPUT);
        pinMode(pwmPins[k], OUTPUT);
        pinMode(motorIn1[k], OUTPUT);
        pinMode(motorIn2[k], OUTPUT);
    }

    attachInterrupt(digitalPinToInterrupt(encoderA[0]), readEncoder<0>, RISING);
    attachInterrupt(digitalPinToInterrupt(encoderA[1]), readEncoder<1>, RISING);
}

void loop() {
    // Calculate the difference in position between the two motors
    int positionDifference = posi[0] - posi[1];

    // Adjust the speed of each motor based on the position difference
    int speedMotor1 = baseSpeed - positionDifference;
    int speedMotor2 = baseSpeed + positionDifference;

    // Ensure the speed is within the allowable range
    speedMotor1 = constrain(speedMotor1, 0, maxSpeed);
    speedMotor2 = constrain(speedMotor2, 0, maxSpeed);

    // Move motors towards the target position
    for (int k = 0; k < 2; k++) {
        if (posi[k] < targetPosition) {
            if (k == 0) {
                setMotor(1, speedMotor1, pwmPins[k], motorIn1[k], motorIn2[k]); // Motor 1
            } else {
                setMotor(1, speedMotor2, pwmPins[k], motorIn1[k], motorIn2[k]); // Motor 2
            }
        } else {
            setMotor(0, 0, pwmPins[k], motorIn1[k], motorIn2[k]); // Stop motors
        }

        // Print the position for debugging
        Serial.print("Motor ");
        Serial.print(k);
        Serial.print(" Position: ");
        Serial.println(posi[k]);
    }

    delay(100); // Add a delay to avoid too frequent updates
}

void setMotor(int dir, int pwmVal, int pwmPin, int in1, int in2) {
    Serial.print(dir);
    Serial.print(" , ");
    Serial.print(pwmVal);
    Serial.print(" , ");
    Serial.print(pwmPin);
    Serial.print(" , ");
    Serial.print(in1);
    Serial.print(" , ");
    Serial.println(in2);
    
    analogWrite(pwmPin, pwmVal);
    if (dir == 1) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    } else if (dir == -1) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
    } else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
    }
}

template <int j>
void readEncoder() {
    int b = digitalRead(encoderB[j]);
    if (b > 0) {
        posi[j]++;
    } else {
        posi[j]--;
    }
}
