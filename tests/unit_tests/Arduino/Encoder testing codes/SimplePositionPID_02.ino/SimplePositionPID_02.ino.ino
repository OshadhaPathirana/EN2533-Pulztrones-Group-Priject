#include <util/atomic.h>

// A class to compute the control signal
class SimplePID {
  private:
    float kp, kd, ki, umax; // Parameters
    float eprev, eintegral; // Storage

  public:
    // Constructor
    SimplePID() : kp(1), kd(0), ki(0), umax(255), eprev(0.0), eintegral(0.0) {}

    // A function to set the parameters
    void setParams(float kpIn, float kdIn, float kiIn, float umaxIn) {
      kp = kpIn; kd = kdIn; ki = kiIn; umax = umaxIn;
    }

    // A function to compute the control signal
    void evalu(int value, int target, float deltaT, int &pwr, int &dir) {
      // error
      int e = target - value;

      // derivative
      float dedt = (e - eprev) / deltaT;

      // integral
      eintegral = eintegral + e * deltaT;

      // control signal
      float u = kp * e + kd * dedt + ki * eintegral;

      // motor power
      pwr = (int) fabs(u);
      if (pwr > umax) {
        pwr = umax;
      }

      // motor direction
      dir = 1;
      if (u < 0) {
        dir = -1;
      }

      // store previous error
      eprev = e;
    }
};

// How many motors
#define NMOTORS 2

// Pins
const int encoderA[] = {2, 3}; // Encoder A pins
const int encoderB[] = {4, 5}; // Encoder B pins
const int pwmPins[] = {9, 13}; // PWM pins for motor control
const int motorIn1[] = {8, 11}; // Motor direction pins 1
const int motorIn2[] = {10, 12}; // Motor direction pins 2

// Globals
long prevT = 0;
volatile int posi[] = {0, 0};

// PID class instances
SimplePID pid[NMOTORS];

// Duration to run the motors (in microseconds)
const long runTime = 10 * 1000000; // 10 seconds
long startTime;

void setup() {
  Serial.begin(9600);

  for (int k = 0; k < NMOTORS; k++) {
    pinMode(encoderA[k], INPUT);
    pinMode(encoderB[k], INPUT);
    pinMode(pwmPins[k], OUTPUT);
    pinMode(motorIn1[k], OUTPUT);
    pinMode(motorIn2[k], OUTPUT);

    pid[k].setParams(1, 0, 0, 255);
  }

  attachInterrupt(digitalPinToInterrupt(encoderA[0]), readEncoder<0>, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderA[1]), readEncoder<1>, RISING);

  startTime = micros();
}

void loop() {
  // Check if the runtime has exceeded 10 seconds
  if (micros() - startTime > runTime) {
    // Stop the motors
    for (int k = 0; k < NMOTORS; k++) {
      setMotor(0, 0, pwmPins[k], motorIn1[k], motorIn2[k]);
    }
    return;
  }

  // set a constant target position (e.g., a large positive value to keep moving forward)
  int targetPosition = 1000; // Target position for both motors

  // time difference
  long currT = micros();
  float deltaT = ((float)(currT - prevT)) / (1.0e6);
  prevT = currT;

  // Read the position in an atomic block to avoid a potential misread
  int pos[NMOTORS];
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    for (int k = 0; k < NMOTORS; k++) {
      pos[k] = posi[k];
    }
  }

  // loop through the motors
  for (int k = 0; k < NMOTORS; k++) {
    int pwr, dir;
    // evaluate the control signal
    pid[k].evalu(pos[k], targetPosition, deltaT, pwr, dir);
    // signal the motor
    setMotor(dir, pwr, pwmPins[k], motorIn1[k], motorIn2[k]);
  }

  for (int k = 0; k < NMOTORS; k++) {
    Serial.print(targetPosition);
    Serial.print(" ");
    Serial.print(pos[k]);
    Serial.print(" ");
  }
  Serial.println();
}

void setMotor(int dir, int pwmVal, int pwmPin, int in1, int in2) {
  Serial.print("PWM Value: ");
  Serial.println(pwmVal);
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
