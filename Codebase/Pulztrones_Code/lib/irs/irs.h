// irs.h
#ifndef IRS_H
#define IRS_H

#include <QTRSensors.h>
#include "config.h"
#include "motors.h"

// Declare the QTRSensors instance and functions
extern QTRSensors qtr;
extern QTRSensors qtrBack;
const uint16_t SensorCount = 8;  // Define SensorCount as a constant
const uint16_t SensorCountBack = 6;
extern uint16_t sensorValues[8];  // Use a fixed size for sensorValues
extern uint16_t sensorValuesBack[6];


Junction Detect_Junction_type();


// Function declarations
void initIRSensors();
void calibrateIRSensors();
void calibrateIRSensorsForBlack();

int readBlackLinePosition();
int readWhiteLinePosition();
int readBackWhiteLinePosition();
Junction Detect_Junction_type_on_black_line();
Junction Detect_Junction_type_on_Color_line(int number, int COLOR_LINE_THRESHOLD_LEFT, int COLOR_LINE_THRESHOLD_RIGHT);
Junction Detect_jun_condition();

int getAverageSensorReading(int sensorPin, int numReadings);
int isWhiteLineDetected();
Junction Detect_Junction_type_on_dottedLine();


#endif
