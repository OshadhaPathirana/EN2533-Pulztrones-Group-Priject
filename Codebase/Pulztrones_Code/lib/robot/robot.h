#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "motors.h"
#include "encoders.h"
#include "irs.h"
#include "buzzer.h"
#include "config.h"
#include "controller.h"
#include "PID.h"
#include "ArmMechanism.h"
#include "CoinDropper.h"
#include "Ultrasonic.h"
#include "ToF.h"
#include "I2CMUX.h"
#include "color.h"



class Robot;
extern Robot robot;
extern VL53L0X_Multiplexer tofSensors;
extern I2CMUX mux;

class Robot {
    public:
        
        

        Robot();
        void init();

        void set_task(Task newtask);
        void turn_right_90();
        void turn_left_90();
        void turn_left_180();


        void turn_on_led(int PIN);
        void turn_off_led(int PIN);

        String detect_Color();
        
        Task get_task();


        void turn_on_LEDS();
        void turn_off_LEDS();

        void pick_virtual_box();
        void drop_virtual_box();

        void pick_box_and_lift(ArmMechanism &armMechanism);
        void drop_box_and_release(ArmMechanism &armMechanism);

        int check_for_left_wall(Ultrasonic &ultrasonic);

        //int get_front_distance_from_center_tof();

        void turn_Right_90_after_moving_forward();
        void turn_Left_90_after_moving_forward();

        void Color_turn_Right_90_after_moving_forward();
        void Color_turn_Left_90_after_moving_forward();

        void turn_Right_90_after_moving_reverse();
        void turn_Left_90_after_moving_reverse();

        int get_front_distance_from_middle_tof();
        int get_front_distance_from_bottom_tof();
        int get_front_distance_from_top_tof();
        

        void Black_turn_Left_90_after_moving_forward();
        void Black_turn_Right_90_after_moving_forward();

        void turn_left_180_after_Dropping();
    
    private:
        Task task = STOP;
        int red, green, blue; 
};



#endif // ROBOT_H