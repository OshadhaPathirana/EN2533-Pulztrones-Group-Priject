#include "tasks.h"
#include <EEPROM.h>


void start_square() {
    // Code to execute the start square task
    Serial2.println("Starting.......");
}

// Tasks
//Task1
float error_enc_NEW = 0.0;
float correction_enc_NEW = 0.0;

int barcode[12];
int barcode_index = 0;
int stripCounter = 0;
int consecutiveEdges = 0;
int Counting_and_Line_Navigation(){
    int distance = 2000;
    const int target_encoder_count = distance / MM_PER_COUNT;
    int encoder_count_left = 0;
    int encoder_count_right = 0;
   
    int currentColor = 1;
    int previousColor = 1;
    int lastStripStart = 0;
    resetEncoders();

    while(encoder_count_left < target_encoder_count && encoder_count_right < target_encoder_count){
        encoder_count_left = getLeftEncoderCounts();
        encoder_count_right = getRightEncoderCounts();

        float distance = (encoder_count_left + encoder_count_right) * 0.5 * MM_PER_COUNT;

        //int ir = analogRead(LEFT_MARKER_SENSOR);

        int leftSensorValue = analogRead(LEFT_MARKER_SENSOR);
        int rightSensorValue = analogRead(RIGHT_MARKER_SENSOR);
        // white 1
        //blacck 0

        if (leftSensorValue > 100 && rightSensorValue > 100){
            currentColor = 0;
        }
        else{
            currentColor = 1;
        }

        if(currentColor == 1 && previousColor == 0){
            lastStripStart = distance;
            previousColor = 1;
        }
        if(currentColor == 0 && previousColor == 1){
            if(stripCounter == 0){
                stripCounter++;
                previousColor = 0;
                continue;
            }
            float strip_length = distance - lastStripStart;

            if(strip_length < 40 && strip_length > 20){
                consecutiveEdges++;
                barcode[barcode_index] = 0;
                barcode_index++;
            }
            else if(strip_length > 40){
                barcode[barcode_index] = 1;
                barcode_index++;
                consecutiveEdges = 0;
            }

            //barcode[barcode_index] = distance - di;
            //Serial2.println(barcode[barcode_index - 1]);
            //barcode_index++;
            previousColor = 0;

        }

        if(consecutiveEdges == 3){
            break;
        }

        



        // Calculate encoder-based PID
        error_enc_NEW = encoder_count_right - encoder_count_left;
        correction_enc_NEW = PIDEnc(error_enc_NEW);

        // Combine encoder and IR corrections (with priority on encoder)
        float total_correction = correction_enc_NEW;

        // Calculate motor speeds
        float left_speed = BASE_SPEED - 10 + total_correction;
        float right_speed = BASE_SPEED - 10 - total_correction;

        setMotorLPWM(left_speed);
        setMotorRPWM(right_speed);

    }
    // MotorBreak();
    // setMotorLPWM(0);
    // setMotorRPWM(0);
    MoveDistanceForward(40);


    return calculateModulo5(binaryToDecimal(barcode, barcode_index - 3));
}



void execute_MoveToMaze() {
    // Code to execute the white line following task
    Serial2.println("Executing WHITE_LINE_FOLLOW");
    //MoveDistanceForward(40)
    FollowWhiteLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();
    FollowWhiteLineUntilJunction();

}

void execute_maze(int VB_POS) {
    int Gate = 1;
    // Code to execute the maze task
    Serial2.println("Executing MAZE");

    if(VB_POS == 0){
        robot.pick_virtual_box();
        
        MoveDistanceForward_and_not_stop(30);
        FollowWhiteLineUntilJunction();
        robot.drop_virtual_box();

        MoveDistanceReverse_and_not_stop(30);
        MoveReverseUntillJunction();
        robot.turn_Right_90_after_moving_reverse();
        FollowWhiteLineUntilJunction();
        robot.turn_Left_90_after_moving_forward();
        FollowWhiteLineUntilJunction();

        if(robot.get_front_distance_from_middle_tof() < 500){
            // Closed
            robot.turn_Left_90_after_moving_forward();
            FollowWhiteLineUntilJunction();

            robot.pick_virtual_box();
            MoveDistanceReverse_and_not_stop(30);
            MoveReverseUntillJunction();
            MoveDistanceReverse_and_not_stop(30);
            MoveReverseUntillJunction();
            MoveDistanceReverse_and_not_stop(30);
            MoveReverseUntillJunction();
            MoveDistanceReverse_and_not_stop(30);
            MoveReverseUntillJunction();
            robot.drop_virtual_box();

            box_in_lower_position();
        }
        else{
            robot.turn_Left_90_after_moving_forward();
            FollowWhiteLineUntilJunction();

            robot.pick_virtual_box();
            MoveDistanceReverse_and_not_stop(30);
            MoveReverseUntillJunction();
            robot.drop_virtual_box();

            box_in_lower_position();
            
        }
        //dsfsdf
    }
    else{
        //checking the gate
        MoveDistanceForward_and_not_stop(50);
        FollowWhiteLineUntilJunction();
        robot.turn_Right_90_after_moving_forward();
        FollowWhiteLineUntilJunction();
        robot.turn_Left_90_after_moving_forward();

        if(robot.get_front_distance_from_middle_tof() < 250){
            // Gate 1 closed
            Gate = 3;
            Buzzer_Toggle(100);
            delay(200);
            Buzzer_Toggle(100);
            delay(200);
            Buzzer_Toggle(100);
        }
        else{
            Gate = 1;
            Buzzer_Toggle(100);
        }

        //move to zero position
        robot.turn_left_90();
        MoveDistanceForward_and_not_stop(40);
        FollowWhiteLineUntilJunction();
        robot.turn_Left_90_after_moving_forward();
        FollowWhiteLineUntilJunction();
        robot.turn_Left_90_after_moving_forward();
        FollowWhiteLineUntilJunction();

        switch (Gate)
        {
        case 1:
            FollowWhiteLineUntilJunction();
            switch (VB_POS)
            {
            case 1:
                break;
            case 2:
                MoveDistanceForward_and_not_stop(30);
                FollowWhiteLineUntilJunction();

                robot.pick_virtual_box();
                MoveDistanceReverse_and_not_stop(30);
                MoveReverseUntillJunction();
                robot.drop_virtual_box();
                break;
            case 3:
                MoveDistanceForward_and_not_stop(30);
                FollowWhiteLineUntilJunction();
                MoveDistanceForward_and_not_stop(30);
                FollowWhiteLineUntilJunction();

                robot.pick_virtual_box();
                MoveDistanceReverse_and_not_stop(30);
                MoveReverseUntillJunction();
                MoveDistanceReverse_and_not_stop(30);
                MoveReverseUntillJunction();
                robot.drop_virtual_box();
                break;
            case 4:
                MoveDistanceForward_and_not_stop(30);
                FollowWhiteLineUntilJunction();
                MoveDistanceForward_and_not_stop(30);
                FollowWhiteLineUntilJunction();
                MoveDistanceForward_and_not_stop(30);
                FollowWhiteLineUntilJunction();
                
                robot.pick_virtual_box();
                MoveDistanceReverse_and_not_stop(30);
                MoveReverseUntillJunction();
                MoveDistanceReverse_and_not_stop(30);
                MoveReverseUntillJunction();
                MoveDistanceReverse_and_not_stop(30);
                MoveReverseUntillJunction();
                robot.drop_virtual_box();
                break;

            
            default:
                break;
            }
            break;
        case 3:
            switch (VB_POS)
            {
                case 1:
                    robot.pick_virtual_box();
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    robot.drop_virtual_box();
                    break;
                case 2:
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    robot.pick_virtual_box();
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    robot.drop_virtual_box();
                    break;
                case 3:
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    break;
                case 4:
                   MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    MoveDistanceForward_and_not_stop(30);
                    FollowWhiteLineUntilJunction();
                    robot.pick_virtual_box();
                    MoveDistanceReverse_and_not_stop(30);
                    MoveReverseUntillJunction();
                    robot.drop_virtual_box();
                    break;
            }
        default:
            break;
        }
        box_upper_position();
    }
    
}

void box_in_lower_position(){
    MoveDistanceReverse_and_not_stop(50);
    MoveReverseUntillJunction();
    robot.turn_Left_90_after_moving_reverse();
    FollowWhiteLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();
    FollowWhiteLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();
    FollowWhiteLineUntilJunction();

    robot.pick_virtual_box();
    MoveDistanceForward(30);
    //Todo: Change this to distance in the arena
    FollowWhiteLine_GivenDistance(80);
    MoveDistanceForward(140);
    robot.drop_virtual_box();
}

void blue_open(){
    MoveDistanceReverse_and_not_stop(30);
    MoveReverseUntillJunction();
    turnLeft90();
    FollowWhiteLineUntilJunction();
    turnRight90();
    FollowWhiteLineUntilJunction();
    turnRight90();
    FollowWhiteLineUntilJunction();
    robot.pick_virtual_box();
    MoveDistanceReverse_and_not_stop(30);
    MoveReverseUntillJunction();
}

void box_upper_position(){
    MoveDistanceReverse_and_not_stop(30);
    MoveReverseUntillJunction();
    robot.turn_Left_90_after_moving_reverse();
    FollowWhiteLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();
    FollowWhiteLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();
    FollowWhiteLineUntilJunction();
    robot.pick_virtual_box();
    MoveDistanceReverse_and_not_stop(30);
    MoveReverseUntillJunction();
    MoveDistanceReverse(180);
    robot.drop_virtual_box();

    //robot.turn_left_180();
    robot.turn_right_90();
    //MoveDistanceForward(15);
    robot.turn_right_90();
    
}


int number = 4;
int x_pos = 0;
int y_pos = 0;


bool matrix[3][4] = {
  {false, false, false, false},   // First row
  {false, false, false, false},   // Second row
  {false, false, false, false}    // Third row
};


int order = 0;
String exectute_colorLineFollowing(){
    String color = robot.detect_Color();
    if(color == "RED"){
        robot.turn_on_led(RED_LED);
        robot.turn_off_led(BLUE_LED);
        //SENSORS_ON_LINE_FOR_JUNCTION_CHECK_COLOUR = 5;
        number = 4;//5
        
    }
    else if(color == "BLUE"){
        robot.turn_on_led(BLUE_LED);
        robot.turn_off_led(RED_LED);
        number = 3;//4
        
        //SENSORS_ON_LINE_FOR_JUNCTION_CHECK_COLOUR = 4;
    }
    else{
        robot.turn_off_led(BLUE_LED);
        robot.turn_off_led(RED_LED);
    }
    delay(1000);
    robot.turn_off_led(BLUE_LED);
    robot.turn_off_led(RED_LED);

    if(color == "BLUE"){
        order = 1;
    }
    else{
        order = 0;
    }
    EEPROM.put(0, order);

   //do color line following here
    MoveDistanceForward(100);

    while(true){
        Junction junction = FollowColorLineUntilJunction(number, color);
        Serial2.println(junction);

        if(junction == T_Junction){
            MoveDistanceForward(50);
        }
        else if(junction == Left){
            robot.turn_Left_90_after_moving_forward();
        }
        else if(junction == Right){
            robot.turn_Right_90_after_moving_forward();
        
        }
        else if(junction == Straight){
            MoveDistanceForward(15);
        }
    }


    return color;
}

void execute_box_manup(int assending){
    int box_height;
    int current_box_dest;
    int bottom_tof_dist;

    FollowBlackLineUntilJunction();
    robot.Black_turn_Left_90_after_moving_forward();
    FollowBlackLine_GivenDistance(260);
    //  bottom_tof_dist = robot.get_front_distance_from_bottom_tof();
    //  Serial2.println(bottom_tof_dist);
    //   if (bottom_tof_dist < 200){
    //     FollowBlackLine_GivenDistance(bottom_tof_dist-70);
    //   }
    //   else{
    //    Serial2.println("No box detected"); 
    //   }
    
    box_height = meassure_box_height();
    Serial2.println("box height- ");
    Serial2.println(box_height);
    current_box_dest = find_destination(0, box_height, assending);
    Serial2.println("next_box_path- ");
    Serial2.println(current_box_dest);
    
    robot.pick_box_and_lift(armMechanism);
    goto_box_destination(current_box_dest);
    // robot.drop_box_and_release(armMechanism);

    ///////////////////////////////////
    box_height = meassure_box_height();
    Serial2.println("box height- ");
    Serial2.println(box_height);
    current_box_dest = find_destination(1, box_height, assending);
    Serial2.println("next_box_path- ");
    Serial2.println(current_box_dest);
    
    robot.pick_box_and_lift(armMechanism);
    goto_box_destination(current_box_dest);
    // robot.drop_box_and_release(armMechanism);

    //////////////////////////////////
    box_height = meassure_box_height();
    Serial2.println("box height- ");
    Serial2.println(box_height);
    current_box_dest = find_destination(2, box_height, assending);
    Serial2.println("next_box_path- ");
    Serial2.println(current_box_dest);
    
    robot.pick_box_and_lift(armMechanism);
    goto_box_destination(current_box_dest);
    // robot.drop_box_and_release(armMechanism);
}



void executeDashedLine(){//execute the task 
    FollowWhiteLineUntilJunction();
}



void executePortalNavigation(){

armMechanism.moveto_TOF_Reading_Position();
// Constants for gate detection
const int GATE_DISTANCE_THRESHOLD = 300; // Distance threshold in mm
const int DELAY_AFTER_GATE = 500;        // Delay in ms
int n=0;

while(n == 0){

 
    // Read distance from the middle ToF sensor
    int middleDistance = robot.get_front_distance_from_bottom_tof();
    Serial2.print("Bottom ToF Distance: ");

    if ( middleDistance <= GATE_DISTANCE_THRESHOLD) {
        // Gate detected
        Serial2.print(middleDistance);
        Serial2.println(" mm (Gate detected)");

        // Wait until the gate is no longer detected
        while (true) {
            middleDistance = robot.get_front_distance_from_bottom_tof();
            if (middleDistance > GATE_DISTANCE_THRESHOLD ) {
                // Gate no longer detected
                Serial2.println("Gate no longer detected");
                delay(DELAY_AFTER_GATE); 
                MoveDistanceForward_in_uneven(350);
                //MoveDistanceForward(350.0); 
                n=1  ;   
                break;                   
            }
        }
    }  
    else {
        // No gate detected
        Serial2.print(middleDistance);
        Serial2.println(" mm (No gate detected)");
    }
}
armMechanism.moveToRestPosition();
}
//execute the task 



int distance = 180;
void executeChamberInsertion(){//execute the task 
    robot.Black_turn_Left_90_after_moving_forward();
    MoveDistanceForward(150);
    FollowBlackLineUntilJunction();
    robot.turn_Left_90_after_moving_forward();
    // FollowBlackLineUntilJunction();
    distance = FollowBlackLineUntilJunction_and_return_Distance();

    robot.pick_box_and_lift(armMechanism);
    robot.turn_left_180();
    FollowBlackLineUntilJunction();
    robot.turn_Left_90_after_moving_forward();

}


void executeHiddenTask(){//execute the task 
//////////////////////////////////////////
//Hidden Task///////////////
//////////////////////////////////////////


    FollowBlackLineUntilJunction();
    MoveDistanceForward_and_not_stop(30);

    FollowBlackLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();

    //try to add real distance here
    int dist = distance - 15;
    FollowBlackLine_GivenDistance(100);
    armMechanism.moveToRestPosition_without_off();
    delay(1000);
    armMechanism.moveToPickupPosition();
    robot.turn_left_180_after_Dropping();
    FollowBlackLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();



    FollowBlackLineUntilJunction();
    MoveDistanceForward_and_not_stop(30);
    //Todo: adjust this
    FollowBlackLine_GivenDistance(160);
    robot.drop_box_and_release(armMechanism);

    //Marisiya
    MoveDistanceReverse(70);
    armMechanism.closeGripper();
    FollowBlackLine_GivenDistance(30);




    robot.turn_left_180_after_Dropping();
    armMechanism.moveToRestPosition();
    FollowBlackLineUntilJunction();
    robot.turn_Right_90_after_moving_forward();
}
void executeUnevenTerrain(){
    FollowBlackLineUntilJunction();
    armMechanism.moveToPickupPosition();
    MoveDistanceForward(200);
    MoveDistanceForward_in_uneven(650);

    // turnLeft90_in_uneven();

    // MoveDistanceForward_in_uneven(500);
    // turnRight90_in_Uneven();
    // MoveDistanceForward_in_uneven(100);
    // turnRight90_in_Uneven();
    // MoveDistanceForward_in_uneven(200);
    
    coinDropper.dropCoin();

}
