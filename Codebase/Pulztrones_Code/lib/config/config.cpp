#include "config.h"

int binaryToDecimal(int binaryArray[], int arraySize) {
    int decimalValue = 0;
    for (int i = 0; i < arraySize; i++) {
        // Calculate the decimal value by multiplying each bit by its positional value
        decimalValue = (decimalValue << 1) | binaryArray[i];
    }
    return decimalValue;
}

int calculateModulo5(int decimalNumber) {
    return decimalNumber % 5;
}