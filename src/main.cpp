#include "LedControl.h"

/*
    Pins
    ----------
    12 - DIN
    11 - CLK
    10 - CS

    7 - Left Button
    8 - Right Button
    4 - Fire Button
*/

// LED Matrix
#define DIN 12
#define CLK 11
#define CS 10

// Buttons

#define rightBtn 7
#define leftBtn 8
#define fireBtn 4

// Defines led control
LedControl lc=LedControl(12,11,10,1);

// Player variables
uint8_t playerPos = 0b00111000;

// Keeps track of time when a button was released to add a delay
unsigned long buttonPressedTime = 0;

// Delay between button presses (ms)
float buttonDelay = 200; 

// Function Prototypes
void MoveLeft();
void MoveRight();
void Fire();

void setup() {
    Serial.begin(9600);

    // Sets up buttons as inputs
    pinMode(leftBtn, INPUT_PULLUP);
    pinMode(rightBtn, INPUT_PULLUP);
    pinMode(fireBtn, INPUT_PULLUP);

    // Wakes up MAX7219
    lc.shutdown(0,false);
    
    // Sets brightness from 0 - 15
    lc.setIntensity(0,0);

    // Clears display 0
    lc.clearDisplay(0);

    // Sets initial position
    lc.setColumn(0, 0, playerPos);

}

void loop() { 
    // Movement input is only checked if more than 500ms has passed since last input
    if (millis() - buttonPressedTime > buttonDelay){
        if (!digitalRead(leftBtn)){
            buttonPressedTime = millis();
            Serial.println("Moving left");
            MoveLeft();
            lc.setColumn(0, 0, playerPos);
        }

        if (!digitalRead(rightBtn)){
            buttonPressedTime = millis();
            Serial.println("Moving right");
            MoveRight();
            lc.setColumn(0, 0, playerPos);
        }
    }

    if (!digitalRead(fireBtn)){
        Fire();
    }

    Serial.println(playerPos);
}

// Functions
void MoveLeft(){
    if (playerPos < 0b11100000){
        playerPos = playerPos << 1; // shifts player pos by one bit
    }
}

void MoveRight(){
    if (playerPos > 0b00000111){
        playerPos = playerPos >> 1; // shifts player pos by one bit
    }
}

void Fire(){
    
}