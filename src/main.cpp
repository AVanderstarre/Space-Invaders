#include "LedControl.h"

/*
    Bottom left of matrix is (0,0), increases up and to the right

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
int8_t playerLeft = 2;
uint8_t playerCenter = 3; 
int8_t playerRight = 4;

// Projectiles
bool projSpawned = false;
uint8_t projRow = 1; // row the bullet is currently in (starts at 1)
uint8_t projXB;  // projectile x coord (binary value that represents horizontal position)
uint8_t projX; // cartesian coordinate
uint8_t projY; // cartesian coordinate
unsigned long projLastUpdate; // time of last projectile update
unsigned int projUpdateDelay = 30; // delay between projectile updates (ms)

// Alient Variables
bool alienSpawned = false;
int8_t alienLeft;
int8_t alienRight;
uint8_t alienY;
bool movingRight = true; //keeps track of aliens direction
unsigned long alienLastUpdate; // time when last alien update occured
uint8_t alienUpdateDelay = 200; // delay between alien updates (ms)


// Keeps track of time when a button was released to add a delay
unsigned long buttonPressedTime = 0;

// Delay between button presses (ms)
float buttonDelay = 200; 

// Function Prototypes
void MoveLeft();
void MoveRight();
void UpdatePlayerPos();
void ResetPlayerLeds();
void Fire();
void UpdateProjectilePos();
void CheckCollision();
void PrintPlayerPos();

void SpawnAlien();
void MoveAlien();
void UpdateAlienPos();
void ResetAlienLeds();

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
    UpdatePlayerPos();

    SpawnAlien();
}

void loop() { 
    // Movement input is only checked if more than 500ms has passed since last input
    if ((millis() - buttonPressedTime) > buttonDelay){
        if (!digitalRead(leftBtn)){
            buttonPressedTime = millis();
            MoveLeft();
        }

        if (!digitalRead(rightBtn)){
            buttonPressedTime = millis();
            MoveRight();
        }

        // Fire should have its own delay separate from the movement buttons
        if (!digitalRead(fireBtn)){
            buttonPressedTime = millis();
            Fire();
        }
    }

    if (projSpawned && (millis() - projLastUpdate) > projUpdateDelay){
        UpdateProjectilePos();
    }

    if (alienSpawned && (millis() - alienLastUpdate) > alienUpdateDelay){
        MoveAlien();
    }
}

// Functions
void MoveLeft(){
    if (playerLeft > -1){
        ResetPlayerLeds();
        playerLeft--;
        playerCenter--;
        playerRight--;
        UpdatePlayerPos();
    }
}

void MoveRight(){
    if (playerRight < 8){
        ResetPlayerLeds();
        playerLeft++;
        playerCenter++;
        playerRight++;
        UpdatePlayerPos();
    }
}

void Fire(){
    // Find the starting point of the bullet
    if (projSpawned == false){
        projSpawned = true;
        projLastUpdate = millis();

        projX = playerCenter;
        projXB  = (int)(pow(2, playerCenter)+0.5); // 0.5 added to account for rounding down
        projY = 1;

        // Serial.print("Spawning proj at ");
        // Serial.print(projX);
        // Serial.print(" ,");
        // Serial.println(projY);

        lc.setLed(0, projX, projY, true); 
    }
}

// Checks for collision between projectile and alien
void CheckCollision(){
    if (projY == alienY && (projX == alienLeft || projX == alienRight)){
        ResetAlienLeds();
        alienSpawned = false;
    }
}

void UpdateProjectilePos(){
    // y = 8 is just above the top row
    if (projY < 8){
        projLastUpdate = millis(); // updates last update time for delay

        lc.setLed(0, projX, projY, false); // removes light at previous location *** change this to toggle the individual light
        
        // increments y
        projY++;
        lc.setLed(0, projX, projY, true);
        CheckCollision(); 
    }else{
        // Proj spawned being false allows us to spawn another 
        projSpawned = false;
    }
}

// Resets player LEDs
void ResetPlayerLeds(){
    lc.setLed(0, playerLeft, 0, false);
    lc.setLed(0, playerCenter, 0, false);
    lc.setLed(0, playerRight, 0, false);
}

// Updates player pos to current player position
void UpdatePlayerPos(){
    lc.setLed(0, playerLeft, 0, true);
    lc.setLed(0, playerCenter, 0, true);
    lc.setLed(0, playerRight, 0, true);
}

void SpawnAlien(){
    // sets start pos of alien
    alienSpawned = true;

    alienLeft = -1;
    alienRight = 0;
    alienY = 7;

    UpdateAlienPos();
}

void MoveAlien(){
    alienLastUpdate = millis();
    if (movingRight){
        ResetAlienLeds();
        alienLeft++;
        alienRight++;
        UpdateAlienPos();  

        if (alienRight < 8){
            UpdateAlienPos();   
        }else{
            // If we reach the end start moving left
            movingRight = false;
        }
    }else{
        ResetAlienLeds();
        alienLeft--;
        alienRight--;
        UpdateAlienPos();

        if (alienRight > 0){
            UpdateAlienPos();   
        }else{
            // If we reach the end start moving left
            movingRight = true;
        }  
    }
}

void ResetAlienLeds(){
    lc.setLed(0, alienLeft, alienY, false);
    lc.setLed(0, alienRight, alienY, false);
}

void UpdateAlienPos(){
    lc.setLed(0, alienLeft, alienY, true);
    lc.setLed(0, alienRight, alienY, true);
}
