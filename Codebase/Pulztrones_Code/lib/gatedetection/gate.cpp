#include <Arduino.h>

#include <QTRSensors.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <VL53L0X.h>

#include "motors.h"
#include "BluetoothDebug.h"
#include "PID.h"
#include "encoders.h"
#include "color.h"
#include "controller.h"
#include "irs.h"
#include "buzzer.h"
#include "robot.h"
#include "I2CMUX.h"
#include "tasks.h"
#include "MenuSystem.h"
#include "CoinDropper.h"
#include "ArmMechanism.h"
#include "Ultrasonic.h"
#include "ToF.h"



// sensor declarations
Robot robot;
I2CMUX mux(0x70);                        // I2C multiplexer at address 0x70
VL53L0X_Multiplexer tofSensors;          // ToF sensor multiplexer

// Constants for gate detection
const int GATE_DISTANCE_THRESHOLD = 250; // Distance threshold in mm
const int DELAY_AFTER_GATE = 500;        // Delay in ms

void setup() {
    initBluetoothDebug();
    initMotorPins();
    initEncoders();
    initIRSensors();
    initBuzzer();

    mux.begin();                         // Initialize I2C multiplexer
    tofSensors.begin(mux);               // Initialize ToF sensors using multiplexer

    Buzzer_Toggle(100);
    delay(2000);

    robot.init();
    Serial.begin(9600);
    Serial.println("Setup complete.");
}

void loop() {
    // Read distance from the middle ToF sensor
    int middleDistance = robot.get_front_distance_from_middle_tof(tofSensors, mux);
    Serial.print("Middle ToF Distance: ");

    if (middleDistance != 0xFFFF && middleDistance <= GATE_DISTANCE_THRESHOLD) {
        // Gate detected
        Serial.print(middleDistance);
        Serial.println(" mm (Gate detected)");

        // Wait until the gate is no longer detected
        while (true) {
            middleDistance = robot.get_front_distance_from_bottom_tof(tofSensors, mux);
            if (middleDistance > GATE_DISTANCE_THRESHOLD || middleDistance == 0xFFFF) {
                // Gate no longer detected
                Serial.println("Gate no longer detected");
                delay(DELAY_AFTER_GATE); 
                //moveRobotForward();      
                break;                   
            }
        }
    } else if (middleDistance == 0xFFFF) {
        // Sensor error 
        Serial.println("Error");
    } else {
        // No gate detected
        Serial.print(middleDistance);
        Serial.println(" mm (No gate detected)");
    }
}

void moveRobotForward() {
    
    Serial.println("Moving robot forward");
    MoveDistanceForward(150.0); // Move forward 
}