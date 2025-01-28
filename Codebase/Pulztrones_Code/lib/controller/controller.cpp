#include "controller.h"
#include "motors.h"
#include "PID.h"
#include "encoders.h"
#include "color.h"
#include "irs.h"
#include "config.h"


void FollowBlackLine() {
    // Read the position of the line (0 to 7000)
    int position = readBlackLinePosition();

    // Calculate the error from the center
    int error = position - 3500;


    // Calculate the PID correction value
    float pidOutput = PIDLine(error);
    //Serial2.println(pidOutput);

    // Calculate motor speeds
    int leftSpeed = BASE_SPEED + pidOutput;
    int rightSpeed = BASE_SPEED - pidOutput;

    // Constrain motor speeds to the valid range
    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);

    // Apply speeds to motors
    moveForward(leftSpeed, rightSpeed);
}

void FollowWhiteLine() {
    // Read the position of the line (0 to 7000)
    int position = readWhiteLinePosition();

    // Normal line following code continues...
    int error = position - 3500;
    float pidOutput = PIDLine(error);
    
    int leftSpeed = BASE_SPEED + pidOutput;
    int rightSpeed = BASE_SPEED - pidOutput;
    
    leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
    
    moveForward(leftSpeed - 20, rightSpeed);
}

void FollowWhiteLineReverse() {
    // Read the position of the line (0 to 7000)
    int position = readBackWhiteLinePosition();

    // Reverse line following code
    int error = position - 2500;
    float pidOutput = PIDLineReverse(error);
    
    // Invert the PID output for reverse movement
    int leftSpeed = -BASE_SPEED - pidOutput;
    int rightSpeed = -BASE_SPEED + pidOutput;
    
    leftSpeed = constrain(leftSpeed, -MAX_SPEED, -MIN_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, -MIN_SPEED);
    
    // Use negative speeds to move backward
    moveForward(leftSpeed + 20, rightSpeed - 20);
}

void FollowWhiteLine_GivenDistance(int distance) {
    const int target_encoder_count = distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;


    resetEncoders();

    while(encoder_count_left < target_encoder_count && encoder_count_right < target_encoder_count){
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        /// Read the position of the line (0 to 7000)
        int position = readWhiteLinePosition();

        // Normal line following code continues...
        int error = position - 3500;
        float pidOutput = PIDLine(error);
        
        int leftSpeed = BASE_SPEED + pidOutput;
        int rightSpeed = BASE_SPEED - pidOutput;
        
        leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
        rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
        
        moveForward(leftSpeed - 20, rightSpeed);

    }
    MotorBreak();
    setMotorLPWM(0);
    setMotorRPWM(0);
}

void FollowBlackLine_GivenDistance(int distance) {
    const int target_encoder_count = distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;


    resetEncoders();

    while(encoder_count_left < target_encoder_count && encoder_count_right < target_encoder_count){
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        /// Read the position of the line (0 to 7000)
        int position = readBlackLinePosition();

        // Normal line following code continues...
        int error = position - 3500;
        float pidOutput = PIDLine(error);
        
        int leftSpeed = BASE_SPEED + pidOutput;
        int rightSpeed = BASE_SPEED - pidOutput;
        
        leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
        rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
        
        moveForward(leftSpeed - 20, rightSpeed);

    }
    MotorBreak();
    setMotorLPWM(0);
    setMotorRPWM(0);
}

Junction FollowWhiteLineUntilJunction(){
    while (true)
    {
        int position = readWhiteLinePosition();
        Junction junction = Detect_Junction_type();
        if(junction != Junction::Straight){
            //MotorBreak();
            setMotorLPWM(0);
            setMotorRPWM(150);
            delay(33);
            setMotorRPWM(0);
            return junction;
        }

        // Normal line following code continues...
        int error = position - 3500;
        float pidOutput = PIDLine(error);
        
        int leftSpeed = BASE_SPEED + pidOutput;
        int rightSpeed = BASE_SPEED - pidOutput;
        
        leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
        rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
        
        moveForward(leftSpeed - 20, rightSpeed);//l -20
    }
    

    


}





float error_enc = 0.0;
float correction_enc = 0.0;

void MoveDistanceForward(float distance){
    const int target_encoder_count = distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;


    resetEncoders();

    while(encoder_count_left < target_encoder_count && encoder_count_right < target_encoder_count){
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        // Calculate encoder-based PID
        error_enc = encoder_count_right - encoder_count_left;
        correction_enc = PIDEnc(error_enc);

        // Combine encoder and IR corrections (with priority on encoder)
        float total_correction = correction_enc;

        // Calculate motor speeds
        float left_speed = BASE_SPEED - 10 + total_correction;
        float right_speed = BASE_SPEED - 10 - total_correction;

        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

    }
    MotorBreak();
    setMotorLPWM(0);
    setMotorRPWM(0);
}

void MoveDistanceForward_and_not_stop(float distance){
    const int target_encoder_count = distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;


    resetEncoders();

    while(encoder_count_left < target_encoder_count && encoder_count_right < target_encoder_count){
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        // Calculate encoder-based PID
        error_enc = encoder_count_right - encoder_count_left;
        correction_enc = PIDEnc(error_enc);

        // Combine encoder and IR corrections (with priority on encoder)
        float total_correction = correction_enc;

        // Calculate motor speeds
        float left_speed = BASE_SPEED - 10 + total_correction;
        float right_speed = BASE_SPEED - 10 - total_correction;

        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

    }
}

void MoveDistanceReverse(float distance) {
    const int target_encoder_count = -distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;

    resetEncoders();

    while(encoder_count_left > target_encoder_count && encoder_count_right > target_encoder_count) {
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        // Calculate encoder-based PID
        error_enc = encoder_count_right - encoder_count_left;
        correction_enc = PIDEnc(error_enc);

        // Combine encoder corrections
        float total_correction = correction_enc;

        // Calculate motor speeds - note the negative sign to move in reverse
        float left_speed = -(BASE_SPEED - total_correction);
        float right_speed = -(BASE_SPEED + total_correction);

        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);
    }
    
    MotorBreak();
    setMotorLPWM(0);
    setMotorRPWM(0);
}

void MoveDistanceReverse_and_not_stop(float distance) {
    const int target_encoder_count = -distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;

    resetEncoders();

    while(encoder_count_left > target_encoder_count && encoder_count_right > target_encoder_count) {
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        // Calculate encoder-based PID
        error_enc = encoder_count_right - encoder_count_left;
        correction_enc = PIDEnc(error_enc);

        // Combine encoder corrections
        float total_correction = correction_enc;

        // Calculate motor speeds - note the negative sign to move in reverse
        float left_speed = -(BASE_SPEED - total_correction);
        float right_speed = -(BASE_SPEED + total_correction);

        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);
    }
}

Junction MoveReverseUntillJunction() {
   
    
    while (true)
    {
        int position = readWhiteLinePosition();
        Junction junction = Detect_Junction_type();
        if(junction != Junction::Straight){
            //MotorBreak();
            setMotorLPWM(0);
            setMotorRPWM(150);
            delay(33);
            setMotorRPWM(0);
            return junction;
        }

        // Read the position of the line (0 to 7000)
        position = readBackWhiteLinePosition();

        // Reverse line following code
        int error = position - 2500;
        float pidOutput = PIDLineReverse(error);

        // Invert the PID output for reverse movement
        int leftSpeed = -BASE_SPEED + 10  - pidOutput;
        int rightSpeed = -BASE_SPEED + 10 + pidOutput;

        leftSpeed = constrain(leftSpeed, -MAX_SPEED, -MIN_SPEED);
        rightSpeed = constrain(rightSpeed, -MAX_SPEED, -MIN_SPEED);

        // Use negative speeds to move backward
        moveForward(leftSpeed + 20, rightSpeed - 20);
    }
    

    



}


void turn(int direction) {
    // Variables
    int target_encoder_diff = COUNTS_PER_90_DEGREE;
    int encoder_diff = 0;
    int left_encoder_current, right_encoder_current;
    float error = 0, last_error = 0, derivative = 0;
    float turn_speed = 0;

    // Reset encoders
    resetEncoders();

    while (encoder_diff < target_encoder_diff) {
        // Get current encoder counts
        left_encoder_current = getLeftEncoderCounts();
        right_encoder_current = getRightEncoderCounts();

        // Calculate encoder difference
        encoder_diff = abs(right_encoder_current - left_encoder_current);

        // Calculate error
        error = target_encoder_diff - encoder_diff;

        // Calculate derivative
        derivative = error - last_error;

        // PD controller
        turn_speed = Kp_TURN * error + Kd_TURN * derivative;

        // Limit turn speed
        turn_speed = fmin(fmax(turn_speed, MIN_TURN_SPEED), MAX_TURN_SPEED);

        // Set motor speeds based on direction
        float left_speed = (direction > 0) ? turn_speed : -turn_speed;
        float right_speed = (direction > 0) ? -turn_speed : turn_speed;

        if(left_speed > 0){
            left_speed = left_speed - 20;
        }
        else{
            left_speed = left_speed + 20;
        }

        // Apply motor speeds
        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

        // Save current error for next iteration
        last_error = error;

        // Small delay to avoid overwhelming the microcontroller
    }

    // Stop motors after reaching the target
    setMotorLPWM(0);
    setMotorRPWM(0);

    // Reset encoders
    resetEncoders();
}

Junction FollowBlackLineUntilJunction(){
    while (true)
    {
        int position = readBlackLinePosition();
        Junction junction = Detect_Junction_type_on_black_line();
        if(junction != Junction::Straight){
            //MotorBreak();
            if(junction == Junction::Right){
                setMotorLPWM(0);
                setMotorRPWM(150);
                delay(33);
                setMotorRPWM(0);
                return junction;
            }
            else if(junction == Junction::Left){
                setMotorLPWM(100);
                setMotorRPWM(0);
                delay(33);
                setMotorLPWM(0);
                return junction;
            }
            else{
                setMotorLPWM(0);
                setMotorRPWM(100);
                delay(33);
                setMotorLPWM(0);
                return junction;
            }
            return junction;
        }

        // Normal line following code continues...
        int error = position - 3500;
        float pidOutput = PIDLine(error);
        
        int leftSpeed = BASE_SPEED + pidOutput;
        int rightSpeed = BASE_SPEED - pidOutput;
        
        leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
        rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
        
        moveForward(leftSpeed - 20, rightSpeed);//l -20
    }
    
}


void turnRight90() {
    // Variables
    int target_encoder_diff = COUNTS_PER_90_DEGREE_RIGHT;
    int encoder_diff = 0;
    int left_encoder_current, right_encoder_current;
    float error = 0, last_error = 0, derivative = 0;
    float turn_speed = 0;

    // Reset encoders
    resetEncoders();

    while (encoder_diff < target_encoder_diff) {
        // Get current encoder counts
        left_encoder_current = getLeftEncoderCounts();
        right_encoder_current = getRightEncoderCounts();

        // Calculate encoder difference
        encoder_diff = abs(right_encoder_current - left_encoder_current);

        // Calculate error
        error = target_encoder_diff - encoder_diff;

        // Calculate derivative
        derivative = error - last_error;

        // PD controller
        turn_speed = Kp_TURN * error + Kd_TURN * derivative;

        // Limit turn speed
        turn_speed = fmin(fmax(turn_speed, MIN_TURN_SPEED), MAX_TURN_SPEED);

        // Set motor speeds based on direction
        float left_speed =  turn_speed;
        float right_speed = -turn_speed;

        left_speed = left_speed - 20;


        // Apply motor speeds
        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

        // Save current error for next iteration
        last_error = error;

        // Small delay to avoid overwhelming the microcontroller
    }

    // Stop motors after reaching the target
    setMotorLPWM(0);
    setMotorRPWM(0);

    // Reset encoders
    resetEncoders();

    
}

void turnRight90_in_Uneven() {
    // Variables
    int target_encoder_diff = COUNTS_PER_90_DEGREE_RIGHT;
    int encoder_diff = 0;
    int left_encoder_current, right_encoder_current;
    float error = 0, last_error = 0, derivative = 0;
    float turn_speed = 0;

    // Reset encoders
    resetEncoders();

    while (encoder_diff < target_encoder_diff) {
        // Get current encoder counts
        left_encoder_current = getLeftEncoderCounts();
        right_encoder_current = getRightEncoderCounts();

        // Calculate encoder difference
        encoder_diff = abs(right_encoder_current - left_encoder_current);

        // Calculate error
        error = target_encoder_diff - encoder_diff;

        // Calculate derivative
        derivative = error - last_error;

        // PD controller
        turn_speed = Kp_TURN * error + Kd_TURN * derivative;

        // Limit turn speed
        turn_speed = fmin(fmax(turn_speed, MIN_TURN_SPEED), MAX_TURN_SPEED);

        // Set motor speeds based on direction
        float left_speed =  turn_speed + 20;
        float right_speed = -turn_speed - 20;

        left_speed = left_speed - 20;


        // Apply motor speeds
        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

        // Save current error for next iteration
        last_error = error;

        // Small delay to avoid overwhelming the microcontroller
    }

    // Stop motors after reaching the target
    setMotorLPWM(0);
    setMotorRPWM(0);

    // Reset encoders
    resetEncoders();

    
}


void turnLeft90() {
    // Variables
    int target_encoder_diff = COUNTS_PER_90_DEGREE_LEFT;
    int encoder_diff = 0;
    int left_encoder_current, right_encoder_current;
    float error = 0, last_error = 0, derivative = 0;
    float turn_speed = 0;

    // Reset encoders
    resetEncoders();

    while (encoder_diff < target_encoder_diff) {
        // Get current encoder counts
        left_encoder_current = getLeftEncoderCounts();
        right_encoder_current = getRightEncoderCounts();

        // Calculate encoder difference
        encoder_diff = abs(right_encoder_current - left_encoder_current);

        // Calculate error
        error = target_encoder_diff - encoder_diff;

        // Calculate derivative
        derivative = error - last_error;

        // PD controller
        turn_speed = Kp_TURN * error + Kd_TURN * derivative;

        // Limit turn speed
        turn_speed = fmin(fmax(turn_speed, MIN_TURN_SPEED), MAX_TURN_SPEED);

        // Set motor speeds based on direction
        float left_speed = -turn_speed;
        float right_speed = turn_speed;

        
                       
        left_speed = left_speed + 20;
        

        // Apply motor speeds
        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

        // Save current error for next iteration
        last_error = error;

        // Small delay to avoid overwhelming the microcontroller
    }

    // Stop motors after reaching the target
    setMotorLPWM(0);
    setMotorRPWM(0);

    // Reset encoders
    resetEncoders();
}

void turnLeft90_in_uneven() {
    // Variables
    int target_encoder_diff = COUNTS_PER_90_DEGREE_LEFT;
    int encoder_diff = 0;
    int left_encoder_current, right_encoder_current;
    float error = 0, last_error = 0, derivative = 0;
    float turn_speed = 0;

    // Reset encoders
    resetEncoders();

    while (encoder_diff < target_encoder_diff) {
        // Get current encoder counts
        left_encoder_current = getLeftEncoderCounts();
        right_encoder_current = getRightEncoderCounts();

        // Calculate encoder difference
        encoder_diff = abs(right_encoder_current - left_encoder_current);

        // Calculate error
        error = target_encoder_diff - encoder_diff;

        // Calculate derivative
        derivative = error - last_error;

        // PD controller
        turn_speed = Kp_TURN * error + Kd_TURN * derivative;

        // Limit turn speed
        turn_speed = fmin(fmax(turn_speed, MIN_TURN_SPEED), MAX_TURN_SPEED);

        // Set motor speeds based on direction
        float left_speed = -turn_speed-20;
        float right_speed = turn_speed+20;

        
                       
        left_speed = left_speed + 20;
        

        // Apply motor speeds
        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

        // Save current error for next iteration
        last_error = error;

        // Small delay to avoid overwhelming the microcontroller
    }

    // Stop motors after reaching the target
    setMotorLPWM(0);
    setMotorRPWM(0);

    // Reset encoders
    resetEncoders();
}


void turnLeft180() {
    // Variables
    int target_encoder_diff = COUNTS_PER_180_DEGREE_LEFT;
    int encoder_diff = 0;
    int left_encoder_current, right_encoder_current;
    float error = 0, last_error = 0, derivative = 0;
    float turn_speed = 0;

    // Reset encoders
    resetEncoders();

    while (encoder_diff < target_encoder_diff) {
        // Get current encoder counts
        left_encoder_current = getLeftEncoderCounts();
        right_encoder_current = getRightEncoderCounts();

        // Calculate encoder difference
        encoder_diff = abs(right_encoder_current - left_encoder_current);

        // Calculate error
        error = target_encoder_diff - encoder_diff;

        // Calculate derivative
        derivative = error - last_error;

        // PD controller
        turn_speed = Kp_TURN * error + Kd_TURN * derivative;

        // Limit turn speed
        turn_speed = fmin(fmax(turn_speed, MIN_TURN_SPEED), MAX_TURN_SPEED);

        // Set motor speeds based on direction
        float left_speed = -turn_speed;
        float right_speed = turn_speed;

        
                       
        left_speed = left_speed + 20;
        

        // Apply motor speeds
        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

        // Save current error for next iteration
        last_error = error;

        // Small delay to avoid overwhelming the microcontroller
    }

    // Stop motors after reaching the target
    setMotorLPWM(0);
    setMotorRPWM(0);

    // Reset encoders
    resetEncoders();
}


float distance_from_counts(int left_counts, int right_counts){
    return (left_counts + right_counts) * 0.5 * MM_PER_COUNT;
}


Junction FollowColorLineUntilJunction(int number, String color){
    int COLOR_LINE_THRESHOLD_LEFT = 500;//450
    int COLOR_LINE_THRESHOLD_RIGHT = 500; //450
    if(color == "RED"){
        // COLOR_LINE_THRESHOLD_LEFT = 300;
        // COLOR_LINE_THRESHOLD_RIGHT = 200;
    }
    else if(color == "BLUE"){
        // COLOR_LINE_THRESHOLD_LEFT = 450;
        // COLOR_LINE_THRESHOLD_RIGHT = 370;
    }
    while (true)
    {
        int position = readWhiteLinePosition();
        Junction junction = Detect_Junction_type_on_Color_line(number, COLOR_LINE_THRESHOLD_LEFT, COLOR_LINE_THRESHOLD_RIGHT);
        if(junction != Junction::Straight){
            //MotorBreak();
            setMotorLPWM(0);
            setMotorRPWM(150);
            delay(33);
            setMotorRPWM(0);
            return junction;
        }
        

        // Normal line following code continues...
        int error = position - 3500;
        float pidOutput = PIDLine(error);
        
        int leftSpeed = BASE_SPEED + pidOutput;
        int rightSpeed = BASE_SPEED - pidOutput;
        
        leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
        rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
        
        moveForward(leftSpeed - 20, rightSpeed);//l -20
    }
    

    


}






void MoveDistanceForward_in_uneven(float distance){
    const int target_encoder_count = distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;


    resetEncoders();

    while(encoder_count_left < target_encoder_count && encoder_count_right < target_encoder_count){
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        // Calculate encoder-based PID
        error_enc = encoder_count_right - encoder_count_left;
        correction_enc = PIDEnc(error_enc);

        // Combine encoder and IR corrections (with priority on encoder)
        float total_correction = correction_enc;

        // Calculate motor speeds
        float left_speed = BASE_SPEED + 25 + total_correction;
        float right_speed = BASE_SPEED + 25 - total_correction;

        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

    }
    MotorBreak();
    setMotorLPWM(0);
    setMotorRPWM(0);
}



float FollowBlackLineUntilJunction_and_return_Distance(){
        int encoder_count_left = 0;
        int encoder_count_right = 0;


        resetEncoders();

    while (true)
    {
        

        int position = readBlackLinePosition();
        Junction junction = Detect_Junction_type_on_black_line();
        if(junction != Junction::Straight){
            //MotorBreak();
            encoder_count_left = getLeftEncoderCounts();
            encoder_count_right = getRightEncoderCounts();

            float length = distance_from_counts(encoder_count_left, encoder_count_right);
            Serial2.println(length);
            
            if(junction == Junction::Right){
                setMotorLPWM(0);
                setMotorRPWM(150);
                delay(33);
                setMotorRPWM(0);
                return length;
            }
            else if(junction == Junction::Left){
                setMotorLPWM(100);
                setMotorRPWM(0);
                delay(33);
                setMotorLPWM(0);
                return length;
            }
            else{
                setMotorLPWM(0);
                setMotorRPWM(100);
                delay(33);
                setMotorLPWM(0);
                return length;
            }
            return length;
        }

        // Normal line following code continues...
        int error = position - 3500;
        float pidOutput = PIDLine(error);
        
        int leftSpeed = BASE_SPEED + pidOutput;
        int rightSpeed = BASE_SPEED - pidOutput;
        
        leftSpeed = constrain(leftSpeed, MIN_SPEED, MAX_SPEED);
        rightSpeed = constrain(rightSpeed, MIN_SPEED, MAX_SPEED);
        
        moveForward(leftSpeed - 20, rightSpeed);//l -20
    }
    
}