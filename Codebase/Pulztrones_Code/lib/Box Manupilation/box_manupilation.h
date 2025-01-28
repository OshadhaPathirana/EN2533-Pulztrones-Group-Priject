#ifndef box_manulipation_H
#define box_manulipation_H

#include <Arduino.h>
#include "config.h"
#include "ArmMechanism.h"

extern ArmMechanism armMechanism;

int find_destination(int current_location, int box_height, int ascending);
int meassure_box_height();
void goto_box_destination(int dest);

#endif