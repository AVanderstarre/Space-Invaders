#include "LedControl.h"

/*
    12 - DIN
    11 - CLK
    10 - CS
*/

// Defines led control
LedControl lc=LedControl(12,11,10,1);

// Delay time (ms)
unsigned long delaytime=100;

// Player variables
uint8_t playerStartPos = 0b111;

void setup() {
    // Wakes up MAX7219
    lc.shutdown(0,false);
    
    // Sets brightness from 0 - 15
    lc.setIntensity(0,0);

    // Clears display 0
    lc.clearDisplay(0);
}

void loop() { 
      lc.setRow(0, 5, playerStartPos);
}