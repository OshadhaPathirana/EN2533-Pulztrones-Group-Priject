#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

enum Task{
            START_SQUARE,
            BARCODE,
            MovetoMaze,
            MAZE,
            COLOR_LINE,//COLORLINE,
            DASHEDLINE,
            PORTALNAVIGATION,
            BOX_MANUPILATION,//BOXARRANGING,
            CHAMBER,
            HIDDENTASK,
            UNEVEN,
            STOP
        };

enum LineColor {
    WHITELINE,
    BLACKLINE,
    REDLINE,
    BLUELINE
};

enum Junction{
    T_Junction,
    Left,
    Right,
    Straight,
    Turn,
    Go_Straight,
    None
};


//***** Common Funcations *****************************************************//
int binaryToDecimal(int binaryArray[], int arraySize);
int calculateModulo5(int decimalNumber);



//***** ROBOT SPECIFICATIONS *****************************************************//
const float WHEEL_DIAMETER_MM = 66.0;
const float WHEEL_BASE_MM = 193.0;
const float ROBOT_RADIUS = WHEEL_BASE_MM / 2.0;

const float COUNTS_PER_REVOLUTION = 824.0;
const float COUNTS_PER_REVOLUTION_RIGHT = 820.0;
const float COUNTS_PER_REVOLUTION_LEFT = 818.0;

const float MM_PER_COUNT = (PI * WHEEL_DIAMETER_MM) / COUNTS_PER_REVOLUTION;
const float MM_PER_COUNT_RIGHT = (PI * WHEEL_DIAMETER_MM) / COUNTS_PER_REVOLUTION_RIGHT;
const float MM_PER_COUNT_LEFT = (PI * WHEEL_DIAMETER_MM) / COUNTS_PER_REVOLUTION_LEFT;




const float DEG_PER_MM_DIFFERENCE = (180.0 / (WHEEL_BASE_MM * PI));
const float RADIANS_PER_DEGREE = 2 * PI / 360.0;
const float DEGREES_PER_RADIAN = 360.0 / 2 * PI;



//***** IO PINS *****************************************************//
const int LEFT_MARKER_SENSOR = A9;
const int RIGHT_MARKER_SENSOR = A8;

const int BLUE_LED = 37;
const int RED_LED = 53;

#define BUTTON_DOWN 34     // Pin for UP/NEXT
#define BUTTON_SELECT 3 // Pin for SELECT
#define BUTTON_UP 36   // Pin for BACK

const int COIN_DROPPER_ON_PIN = 50;
const int ARM_GRIPPER_ON_PIN = 52;

const int COIN_DROPPER_SERVO_PIN = 8;
const int ARM_LIFT_SERVO_PIN = 7;
const int GRIPPER_SERVO_PIN = 6;

const int TRIG_PIN = 32;   // Trigger pin of the ultrasonic sensor
const int ECHO_PIN = 31;  // Echo pin of the ultrasonic sensor







//*** MOTION CONTROL CONSTANTS **********************************************//
// forward motion controller constants
const float FWD_KP = 0.5;
const float FWD_KD = 0;

// Constants
const int COUNTS_PER_90_DEGREE = 1060; // Experimentally determined

const int COUNTS_PER_90_DEGREE_RIGHT = 1100; // Experimentally determined
const int COUNTS_PER_90_DEGREE_LEFT = 1080; // Experimentally determined

const int COUNTS_PER_180_DEGREE_LEFT = 2250;


const float MAX_TURN_SPEED = 100; // Maximum turn speed
const float MIN_TURN_SPEED = 70; // Minimum turn speed to overcome static friction

// PD controller constants
const float Kp_TURN = 0.6; // Proportional gain
const float Kd_TURN = 1; // Derivative gain

// controller constants for the steering controller
const float STEERING_KP = 0.04;
const float STEERING_KD = 0.00;
const float STEERING_ADJUST_LIMIT = 10.0;  // deg/s



const int MOTOR_MAX_PWM = 255;



// encoder polarity is either 1 or -1 and is used to account for reversal of the encoder phases
#define ENCODER_LEFT_POLARITY (1)
#define ENCODER_RIGHT_POLARITY (-1)

// motor polarity is either 1 or -1 and is used to account for reversal of the motor wiring
#define MOTOR_LEFT_POLARITY (1)
#define MOTOR_RIGHT_POLARITY (1)



//***** PERFORMANCE CONSTANTS************************************************//
const int BASE_SPEED = 70;//70
const int MAX_SPEED = 120;
const int MIN_SPEED = 50;

const int LINE_THRESHOLD = 100;

const int WHITE_LINE_THRESHOLD = 100;
const int SENSORS_ON_LINE_FOR_JUNCTION_CHECK = 4;

const int BLACK_LINE_THRESHOLD = 350;//350

const int COLOR_LINE_THRESHOLD = 400;
const int SENSORS_ON_LINE_FOR_JUNCTION_CHECK_COLOUR = 4;
//const int COLOR_LINE_THRESHOLD_LEFT = 450;
//const int COLOR_LINE_THRESHOLD_RIGHT = 460;

//Todo: calibrate these values
const int RED_LINE_THRESHOLD = 100;
const int BLUE_LINE_THRESHOLD = 100;


const int SENSORS_ON_LINE_FOR_LINE_CHECK = 2;


/******************************************************************************
 * ATOMIC OPERATIONS for ATMEGA328 ONLY
 * Since the ATMega328 is an 8 bit processor it is possible that you will end
 * up trying to read a multi-byte quantity that is modified in an interrupt while
 * you are doing the read or write. The result is a corrupt value. 32 bit processors
 * are unlikely to suffer from this since quantities are read in a single operation.
 *
 * The AVR compiler provides a method for you to disable interrupts for the
 * duration of a block of code and then restore the state at the end of the block.
 *
 * It is not enough to simply turn off interrupts and then turn them back on because
 * you need to remember the state of the interrupt enable flag at the start of the
 * block.
 *
 * These macros do this for you and should be either modified for different processors
 * or bypassed if needed.
 *
 * Use like this:
 * ATOMIC {
 * // code to protect
 * }
 *
 */
#if defined(__AVR__)
#include <util/atomic.h>
#define ATOMIC ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
#else
#define ATOMIC
#endif
//***************************************************************************//

// Define multiplexer channels for ToF sensors
#define CHANNEL_BOTTOM  2
#define CHANNEL_MIDDLE  3
#define CHANNEL_TOP     4


#endif
