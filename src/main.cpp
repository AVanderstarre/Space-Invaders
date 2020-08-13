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
LedControl lc= LedControl(12,11,10,1);

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
void PrintPlayerPos();

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
            MoveLeft();
        }

        if (!digitalRead(rightBtn)){
            buttonPressedTime = millis();
            MoveRight();
 
        }
    }

    if (!digitalRead(fireBtn)){
        Fire();
    }
}

// Functions
void MoveLeft(){
    if (playerPos <= 0b11100000 && playerPos != 0b11000000){
        playerPos = playerPos << 1; // shifts player pos by one bit

        // makes bit come back if it went of of the edge
        if (playerPos == 0b110){
            playerPos = 0b111;
        }
 
        lc.setColumn(0, 0, playerPos);
        PrintPlayerPos();
    }
}

void MoveRight(){
    if (playerPos >= 0b00000111){
        playerPos = playerPos >> 1; // shifts player pos by one bit

       // makes bit come back if it went of of the edge
        if (playerPos == 0b01100000){
            playerPos = 0b11100000;
        }

        lc.setColumn(0, 0, playerPos);
        PrintPlayerPos();
    }
}

void Fire(){
    // Find the starting point of the bullet

}

void PrintPlayerPos(){
    Serial.print("Pos is ");
    Serial.print(playerPos, BIN);
    Serial.print("(");
    Serial.print(playerPos);
    Serial.println(")");
}
