#include <Arduino.h>
#include "irs.h"


// Define QTR instance and global variables
QTRSensors qtr;
QTRSensors qtrBack;
QTRSensors qtrBlack;
uint16_t sensorValues[SensorCount];  // Define sensorValues with a fixed size
uint16_t sensorValuesBack[SensorCountBack];
uint16_t sensorValuesBlack[SensorCount];


// Initialize the IR sensors
void initIRSensors() {
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);

    qtrBack.setTypeAnalog();
    qtrBack.setSensorPins((const uint8_t[]){A10, A13, A11, A12, A15, A13}, SensorCountBack);

    qtrBlack.setTypeAnalog();
    qtrBlack.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);

}




// Calibrate the IR sensors
void calibrateIRSensors() {
    Serial2.println("Starting Blue calibration...");

    for (uint8_t i = 0; i < 250; i++) {
        qtr.calibrate();
        qtrBack.calibrate();
        delay(20);

        // Print the current calibration progress
        if (i % 50 == 0) {  // Print every 50 iterations
            Serial2.print("Calibration progress: ");
            Serial2.print(i);
            Serial2.println("/250");
        }
    }

    Serial2.println("Calibration completed.");
}

void calibrateIRSensorsForBlack() {
    Serial2.println("Starting Black calibration...");

    for (uint8_t i = 0; i < 250; i++) {
        qtrBlack.calibrate();

        delay(20);

        // Print the current calibration progress
        if (i % 50 == 0) {  // Print every 50 iterations
            Serial2.print("Calibration progress: ");
            Serial2.print(i);
            Serial2.println("/250");
        }
    }

    Serial2.println("Calibration completed.");
}

// Read black line position
int readBlackLinePosition() {
    return qtrBlack.readLineBlack(sensorValues);
}

int readWhiteLinePosition(){
    return qtr.readLineWhite(sensorValues);
}

Junction Detect_Junction_type(){
    //bool JunctionDetected = false;
    int numberOfSensorsOnWhite = 0;

    for(int i = 0; i < SensorCount; i++){
        if(sensorValues[i] < WHITE_LINE_THRESHOLD){
            numberOfSensorsOnWhite++;
        }
    }

    if(numberOfSensorsOnWhite > SENSORS_ON_LINE_FOR_JUNCTION_CHECK){
        //MotorBreak();
        int leftSensorValue = analogRead(LEFT_MARKER_SENSOR);
        int rightSensorValue = analogRead(RIGHT_MARKER_SENSOR);

        if (leftSensorValue < WHITE_LINE_THRESHOLD && rightSensorValue < WHITE_LINE_THRESHOLD){
            return T_Junction;
        }
        else if (leftSensorValue < WHITE_LINE_THRESHOLD){
            return Left;
        }
        else if (rightSensorValue < WHITE_LINE_THRESHOLD){
            return Right;
        }
        else{
            return Straight;
        }
    }
    else{
        return Straight;
    }
}

Junction Detect_Junction_type_on_black_line(){
    //bool JunctionDetected = false;
    int numberOfSensorsOnBlack = 0;

    for(int i = 0; i < SensorCount; i++){
        if(sensorValues[i] > BLACK_LINE_THRESHOLD){
            numberOfSensorsOnBlack++;
        }
    }

    if(numberOfSensorsOnBlack > SENSORS_ON_LINE_FOR_JUNCTION_CHECK){//+1
        //MotorBreak();
        int leftSensorValue = analogRead(LEFT_MARKER_SENSOR);
        int rightSensorValue = analogRead(RIGHT_MARKER_SENSOR);

        if (leftSensorValue > BLACK_LINE_THRESHOLD && rightSensorValue > BLACK_LINE_THRESHOLD){
            return T_Junction;
        }
        else if (leftSensorValue > BLACK_LINE_THRESHOLD){
            return Left;
        }
        else if (rightSensorValue > BLACK_LINE_THRESHOLD){
            return Right;
        }
        else{
            return Straight;
        }
    }
    else{
        return Straight;
    }
}

int readBackWhiteLinePosition(){
    return qtrBack.readLineWhite(sensorValuesBack);
}

Junction Detect_Junction_type_on_Color_line(int number, int COLOR_LINE_THRESHOLD_LEFT, int COLOR_LINE_THRESHOLD_RIGHT){
    //bool JunctionDetected = false;
    int numberOfSensorsOnWhite = 0;

    for(int i = 0; i < SensorCount; i++){
        if(sensorValues[i] < 200){
            numberOfSensorsOnWhite++;
        }
    }

    if(numberOfSensorsOnWhite > number){
        MotorBreak();
        int leftSensorValue = getAverageSensorReading(LEFT_MARKER_SENSOR, 8);
        int rightSensorValue = getAverageSensorReading(RIGHT_MARKER_SENSOR, 8);

        // int leftSensorValue = sensorValues[0];
        // int rightSensorValue = sensorValues[7];


        if (leftSensorValue < COLOR_LINE_THRESHOLD_LEFT && rightSensorValue < COLOR_LINE_THRESHOLD_RIGHT){
            return T_Junction;
        }
        else if (leftSensorValue < COLOR_LINE_THRESHOLD_LEFT){
            return Left;
        }
        else if (rightSensorValue < COLOR_LINE_THRESHOLD_RIGHT){
            return Right;
        }
        else{
            return Straight;    
        }
    }
    else{
        return Straight;
        
    }
}

int getAverageSensorReading(int sensorPin, int numReadings) {
    int total = 0;

    for (int i = 0; i < numReadings; i++) {
        total += analogRead(sensorPin);
        delay(5); // Small delay between readings to stabilize sensor values
    }

    return total / numReadings;
}


int isWhiteLineDetected() {
    int numberOfSensorsOnWhite = 0;

    for (int i = 0; i < SensorCount; i++) {
        if (sensorValues[i] < WHITE_LINE_THRESHOLD) {
            numberOfSensorsOnWhite++;
        }
    }

    // Check if enough sensors detect the white line
    if (numberOfSensorsOnWhite == 8)
    {
        return 7;
    }
    else if(numberOfSensorsOnWhite > SENSORS_ON_LINE_FOR_LINE_CHECK){
        return 1;
    }
    else{
        return 0;
    }
    
    
}



Junction Detect_Junction_type_on_dottedLine(){
    //bool JunctionDetected = false;
    int numberOfSensorsOnWhite = 0;

    for(int i = 0; i < SensorCount; i++){
        if(sensorValues[i] < WHITE_LINE_THRESHOLD){
            numberOfSensorsOnWhite++;
        }
    }

    Serial2.println(numberOfSensorsOnWhite);

    if(numberOfSensorsOnWhite > SENSORS_ON_LINE_FOR_JUNCTION_CHECK){
        //MotorBreak();
        int leftSensorValue = analogRead(LEFT_MARKER_SENSOR);
        int rightSensorValue = analogRead(RIGHT_MARKER_SENSOR);

        if (leftSensorValue < WHITE_LINE_THRESHOLD && rightSensorValue < WHITE_LINE_THRESHOLD){
            return T_Junction;
        }
        else if (leftSensorValue < WHITE_LINE_THRESHOLD){
            return Left;
        }
        else if (rightSensorValue < WHITE_LINE_THRESHOLD){
            return Right;
        }
        else{
            return Straight;
        }
    }
    else{
        return Straight;
    }
}