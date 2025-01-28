#include "buzzer.h"

int BUZZER_PIN = 30;


void initBuzzer() {
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}


void Buzzer_On()
{
    digitalWrite(BUZZER_PIN, HIGH);
}

// Turn off the buzzer (PC15 low)
void Buzzer_Off()
{
    digitalWrite(BUZZER_PIN, LOW);
}

// Toggle the buzzer state with a specified delay
void Buzzer_Toggle(uint32_t t)
{
    Buzzer_On();
    delay(t);
    Buzzer_Off();
    delay(t);
}

void Buzzer_UniquePattern()
{
    // Pattern: Short-Short-Long-Short-Long
    // Total duration: 1000ms (1 second)

    Buzzer_On();
    delay(100);  // 100ms on
    Buzzer_Off();
    delay(100);  // 100ms off

    Buzzer_On();
    delay(100);  // 100ms on
    Buzzer_Off();
    delay(100);  // 100ms off

    Buzzer_On();
    delay(200);  // 200ms on
    Buzzer_Off();
    delay(100);  // 100ms off

    Buzzer_On();
    delay(100);  // 100ms on
    Buzzer_Off();
    delay(100);  // 100ms off

    Buzzer_On();
    delay(200);  // 200ms on
    Buzzer_Off();
    // No delay at the end to make it exactly 1 second
}