#include "MenuSystem.h"
#include "tasks.h"
#include "EEPROM.h"

MenuSystem::MenuSystem(uint8_t screenWidth, uint8_t screenHeight, uint8_t oledReset, uint8_t oledAddr) :
    display(screenWidth, screenHeight, &Wire, oledReset),
    currentState(MAIN_MENU),
    currentSelection(0),
    lastDebounceTime(0),
    debounceDelay(50) {}

void MenuSystem::begin() {
    mux.selectChannel(0);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (1); // Handle OLED initialization failure
    }
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.display();

    navigateMenu(true);
    updateDisplay();
}
void MenuSystem::do_tasks() {
    while (robot.get_task() != STOP) {
        switch (robot.get_task()) {
            case START_SQUARE:
                start_square();
                robot.set_task(BARCODE); 
                break;
            case BARCODE:
                VB_POS = Counting_and_Line_Navigation();
                robot.set_task(MovetoMaze);
                break;
            case MovetoMaze:
                execute_MoveToMaze();
                robot.set_task(MAZE);
                break;
            case MAZE:
                //VB_POS = 2;
                execute_maze(VB_POS);
                robot.set_task(COLOR_LINE);
                break;
            case COLOR_LINE:
                color = exectute_colorLineFollowing();
                if(color == "BLUE"){
                    order = 1;
                }
                else{
                    order = 0;
                }
                EEPROM.put(0, order);
                robot.set_task(DASHEDLINE);
                break;
            case DASHEDLINE:
                executeDashedLine();
                robot.set_task(PORTALNAVIGATION);
                break;
            case PORTALNAVIGATION:
                executePortalNavigation();
                robot.set_task(BOX_MANUPILATION);
                break;
            case BOX_MANUPILATION:
                EEPROM.get(0,order);
                execute_box_manup(order);
                robot.set_task(CHAMBER);
                break;
            case CHAMBER:
                executeChamberInsertion();
                robot.set_task(HIDDENTASK);
                break;
            case HIDDENTASK:
                executeHiddenTask();
                robot.set_task(UNEVEN);
                break;
            case UNEVEN:
                executeUnevenTerrain();
                robot.set_task(STOP);
                break;

            case STOP:
                robot.set_task(STOP);
                break;
            default:
                robot.set_task(STOP);
                break;
        }
    }
}

void MenuSystem::updateDisplay() {
    display.clearDisplay();
    display.setCursor(0, 0);

    switch (currentState) {
        case MAIN_MENU:
            renderMenu(mainMenuOptions, 4);
            break;
        case TASKS_SUBMENU_PAGE_1:
            renderMenu(tasksSubmenuPage1, 7); // 5 tasks + "Next >"
            break;
        case TASKS_SUBMENU_PAGE_2:
            renderMenu( tasksSubmenuPage2, 7); // 5 tasks + "< Back"
            break;
        case SENSOR_DIAGNOSTICS:
            renderMenu(sensorDiagnosticOptions, 3);
            break;
        case HARDWARE_TESTING:
            renderMenu(hardwareTestingOptions, 4);
            break;
        case SYSTEM_SETTINGS:
            renderMenu(systemSettingsOptions, 4);
            break;
        default:
            display.println("Invalid State");
            break;
    }
    mux.selectChannel(0);
    display.display();
}


void MenuSystem::renderMenu(const char** options, int optionCount) {
    for (int i = 0; i < optionCount; i++) {
        if (i == currentSelection) {
            display.setTextColor(BLACK, WHITE);  // Highlight selection
        } else {
            display.setTextColor(WHITE);
        }
        display.println(options[i]);
    }
}

void MenuSystem::navigateMenu(bool isUpButton) {
    unsigned long currentTime = millis();
    if ((currentTime - lastDebounceTime) < debounceDelay) return; // Debounce
    lastDebounceTime = currentTime;

    int menuSize = 5;  // Default to main menu size
    switch (currentState) {
        case MAIN_MENU: menuSize = 5; break;
        case TASKS_SUBMENU_PAGE_1: 
        case TASKS_SUBMENU_PAGE_2:
           menuSize = 7; 
           break;
        case SENSOR_DIAGNOSTICS: menuSize = 3; break;
        case HARDWARE_TESTING: menuSize = 4; break;
        case SYSTEM_SETTINGS: menuSize = 4; break; //continue 
    }

    if (isUpButton) {
        currentSelection = (currentSelection - 1 + menuSize) % menuSize;
    } else {
        currentSelection = (currentSelection + 1) % menuSize;
    }
}

void MenuSystem::selectMenuItem() {
    switch (currentState) {
        case MAIN_MENU:
            if (currentSelection == 4) return; // "Back" option, do nothing
            currentState = static_cast<MenuState>(currentSelection + 1); // Transition to selected submenu
            currentSelection = 0;
            break;

        case TASKS_SUBMENU_PAGE_1:
            if (currentSelection == 5) { // "Next >" option
                currentState = TASKS_SUBMENU_PAGE_2;
                currentSelection = 0;
            } else if (currentSelection == 6) { // "< Back" option
                currentState = MAIN_MENU;
                currentSelection = 0;
            } else if(currentSelection==0) {
               Execute_start_square(); // Execute task from Page 1
            }
            else if(currentSelection==1) {
               Execute_bar_code(); // Execute task from Page 1
            }
             else if(currentSelection==2) {
                Execute_Move_To_Maze();
             }
            else if(currentSelection==3) {
                Maze();
            }
            else if (currentSelection==4){
                Ex_ColorLine();
            }
            do_tasks();
            break;

        case TASKS_SUBMENU_PAGE_2:
            if (currentSelection == 6) { // "< Back" option
                currentState = TASKS_SUBMENU_PAGE_1;
                currentSelection = 0;
            } else if (currentSelection == 0) { // "< Back" to MAIN_MENU
               Ex_DashedLine();
            } 
            else if (currentSelection==1){
                Ex_PortalNavigation();
            }
            else if(currentSelection==2){
                Ex_BoxArranging();
            }
            else if (currentSelection==3){
                ChamberInsertion();
            }
            else if (currentSelection==4){
                Ex_HiddenTask();
            }
            else if (currentSelection==5){
                Ex_UnevenTerrain();
            }
            do_tasks();
            break;

        case SENSOR_DIAGNOSTICS:
            if (currentSelection == 2) { // "< Back"
                currentState = MAIN_MENU;
                currentSelection = 0;
            } else {
                executeDiagnostic(); // Run diagnostics
            }
            break;

        case HARDWARE_TESTING:
            if (currentSelection == 3) { // "< Back"
                currentState = MAIN_MENU;
                currentSelection = 0;
            } else {
                executeHardwareTest(); // Run hardware test
            }
            break;

        case SYSTEM_SETTINGS:
            if (currentSelection == 3) { // "< Back"
                currentState = MAIN_MENU;
                currentSelection = 0;
            } else {
                updateSetting(); // Update system settings
            }
            break;

        default:
            currentState = MAIN_MENU;
            currentSelection = 0;
            break;
    
    }
    currentSelection=0;
    //updateDisplay();
}

void MenuSystem::handleInput(int buttonUp, int buttonNext, int buttonSelect) {
    if (digitalRead(buttonUp) == HIGH) {
        navigateMenu(true);
        updateDisplay();
        delay(200);
    }
    if (digitalRead(buttonNext) == HIGH) {
        navigateMenu(false);
        updateDisplay();
        delay(200);
    }
    if (digitalRead(buttonSelect) == HIGH) {
        selectMenuItem();
        updateDisplay();
        delay(200);
    }
}

void MenuSystem::Execute_start_square() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Execute_Start_Square");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(START_SQUARE);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);


}
void MenuSystem :: Execute_bar_code(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Barcode running");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(BARCODE);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}

void MenuSystem ::  Execute_Move_To_Maze(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Moving to Maze");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(MovetoMaze);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);

    }
    void MenuSystem :: Maze(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Solving Maze");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(MAZE);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);

}
void MenuSystem ::Ex_ColorLine(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Color line");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(COLOR_LINE);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem ::Ex_DashedLine(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Dashed line");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(DASHEDLINE);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem ::Ex_PortalNavigation(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Portal Navigation");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(PORTALNAVIGATION);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem :: Ex_BoxArranging(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Box Arranging");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(BOX_MANUPILATION);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem ::  ChamberInsertion(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Chamber insertion");
    display.display();
    delay(1000); // Placeholder for task execution
   robot.set_task(CHAMBER);
   digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem :: Ex_HiddenTask(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Hidden Task");
    display.display();
    delay(1000); // Placeholder for task execution
    robot.set_task(HIDDENTASK);
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem ::  Ex_UnevenTerrain(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Uneven Terrain");
    display.display();
    delay(1000); // Placeholder for task execution
   robot.set_task(UNEVEN);
   digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem :: executeDiagnostic(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("executeDiagnostic");
    display.display();
    delay(1000); // Placeholder for task execution
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem :: executeHardwareTest(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("executeHardwareTest");
    display.display();
    delay(1000); // Placeholder for task execution
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}
void MenuSystem :: updateSetting(){
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("updateSetting");
    display.display();
    delay(1000); // Placeholder for task execution
    digitalWrite(BLUE_LED,HIGH);
    delay(500);
    digitalWrite(BLUE_LED,LOW);
    delay(500);
}


