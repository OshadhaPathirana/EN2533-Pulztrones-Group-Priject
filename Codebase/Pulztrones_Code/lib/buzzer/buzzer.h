#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "config.h"


void initBuzzer();
void Buzzer_On();
void Buzzer_Off();
void Buzzer_Toggle(uint32_t t);
void Buzzer_UniquePattern();


#endif
