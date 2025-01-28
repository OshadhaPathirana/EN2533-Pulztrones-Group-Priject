#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "tasks.h"
#include "I2CMUX.h"

extern I2CMUX mux;

enum MenuState {
    MAIN_MENU,
    TASKS_SUBMENU_PAGE_1,
    TASKS_SUBMENU_PAGE_2,
    SENSOR_DIAGNOSTICS,
    HARDWARE_TESTING,
    SYSTEM_SETTINGS,
    
    TASK_START_SQUARE,
    TASK_BARCODE,
    TASK_MOVE_TO_MAZE,
    TASK_MAZE,
    TASK_COLOR_LINE,
    TASK_DASHED_LINE,
    TASK_PORTAL_NAVIGATION,
    TASK_BOX_ARRANG,
    TASK_CHAMBER_INSERT,
    TASK_HIDDEN_TASK,
    TASK_UNEVEN_TERRAIN,
    TASK_COIN_DROP,
    
    
    DIAG_MPU6050,
    DIAG_TOF_SENSORS,
    
    TEST_SERVOS,
    TEST_MOTORS,
    TEST_BUTTONS_LEDS
};

class MenuSystem {
private:
    Adafruit_SSD1306 display;
    MenuState currentState;
    int currentSelection;
    unsigned long lastDebounceTime;
    const unsigned long debounceDelay;

    const char* mainMenuOptions[4] = {
        "Tasks", "Sensor Diagnostics", "Hardware Testing", 
        "System Settings"
    };

 
    const char* tasksSubmenuPage1[7] = {
        "Start Square", "Barcode", "Move to Maze", "Maze",
        "Color Line", "Next >","<Back"
    };

    const char* tasksSubmenuPage2[7] = {
        "Dashed Line", "Portal Navigation", "Box Arranging",
        "Chamber Insertion", "Hidden Task", "Uneven Terrain","<Back"
    };

    const char* sensorDiagnosticOptions[3] = {
        "MPU6050 Gyroscope", "VL53L0X ToF Sensors", "< Back"
    };

    const char* hardwareTestingOptions[4] = {
        "Servo Motors", "Motors and Wheels", "Buttons and LEDs", "< Back"
    };

    const char* systemSettingsOptions[4] = {
        "Task Parameters", "Sensor Calibration", 
        "Screen Brightness", "< Back"
    };

    void renderMenu(const char** options, int optionCount);
    void Execute_start_square();
    void Execute_bar_code();
    void Execute_Move_To_Maze();
    void Maze();
    void Ex_ColorLine();
    void Ex_DashedLine();
    void Ex_PortalNavigation();
    void Ex_BoxArranging();
    void ChamberInsertion();
    void Ex_HiddenTask();
    void Ex_UnevenTerrain();
    void executeDiagnostic();
    void executeHardwareTest();
    void updateSetting();

public:
    MenuSystem(uint8_t screenWidth, uint8_t screenHeight, uint8_t oledReset, uint8_t oledAddr);
    void begin();
    void updateDisplay();
    void navigateMenu(bool isUpButton);
    void selectMenuItem();
    void handleInput(int buttonUp, int buttonNext, int buttonSelect);
    void do_tasks();

    int VB_POS = 0;
    int order = 0;
    String color = "";
};

#endif // MENUSYSTEM_H