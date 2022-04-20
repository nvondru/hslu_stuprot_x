/*
   Simple demo with DRV8255 driver on CNC shield

   Connect STEP, DIR as indicated!
   Driverslots X and Y on the CNC shield are used

   Adapted 2021/2022 by Gordan Savicic
   based on Copyright (C)2015-2017 Laurentiu Badea

   This file may be redistributed under the terms of the MIT license.
   A copy of this license has been included with this distribution in the file LICENSE.

*/
#include <Arduino.h>
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 20

// Since microstepping is set externally, make sure this matches the selected mode
// Set the jumper to middle position when using MICROSTEPS 4, no jumper = MICROSTEPS 1
// 1=full step, 2=half step etc.
#define MICROSTEPS 4

// Driver in CNC shield X
#define DIR_L 5
#define STEP_L 2

// Driver in CNC shield Y
#define DIR_R 6
#define STEP_R 3

// Define the pin for enable/disable functionality
#define SLEEP 8

// Initialize the driver(s)
BasicStepperDriver stepperL(MOTOR_STEPS, DIR_L, STEP_L, SLEEP);
BasicStepperDriver stepperR(MOTOR_STEPS, DIR_R, STEP_R, SLEEP);

// MultiDriver controller(stepperL, stepperR);
SyncDriver controller(stepperL, stepperR);

const float full_rotation = MOTOR_STEPS * MICROSTEPS;
const float half_rotation = full_rotation / 2;
const float quarter_rotation = full_rotation / 4;

int currentX = 0;
int currentY = 0;

const int boundaryRadius = 80;

const int steps = 2000;
int stepWidth = 20;


void setup()
{   
    // Pass some config to the instances and begin
    stepperL.begin(RPM, MICROSTEPS);
    stepperR.begin(RPM, MICROSTEPS);
 
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    stepperL.setEnableActiveState(LOW);
    stepperR.setEnableActiveState(LOW);

    Serial.begin(115200);
    Serial.println("");
    Serial.println(" ____ _____ _   _ ____  ____   ___   __  __");
    Serial.println("/ ___|_   _| | | |  _ \\|  _ \\ / _ \\  \\ \\/ /");
    Serial.println("\\___ \\ | | | | | | |_) | |_) | | | |  \\  /");
    Serial.println(" ___) || | | |_| |  __/|  _ <| |_| |  /  \\");
    Serial.println("|____/ |_|  \\___/|_|   |_| \\_\\\\___/  /_/\\_\\");
    Serial.println("-----------------------------------------------");

    // enableSteppers();

    drawSomething();

    // disableSteppers();
}


void loop(){
}

void drawSomething(){

    randomSeed(348854);
    int occPositions[steps + 1][2];

    occPositions[0][0] = currentX;
    occPositions[0][1] = currentY;

    for (int i = 1; i < steps + 1; i++)
    {


        int dirX = 0;
        int dirY = 0;
        Serial.println("-------------------");
        Serial.print("Iteration: ");
        Serial.println(i);
        do
        {
            stepWidth = random(2, 25);

            dirX = random(-1, 2);
            Serial.print("dirX: ");
            Serial.println(dirX);

            dirY = random(-1, 2);
            Serial.print("dirY: ");
            Serial.println(dirY);
            Serial.println("-");

  
  
        } while (outOfCardBoundaries(currentX + dirX * stepWidth, currentY + dirY * stepWidth));

        enableSteppers();
        if (dirX == 0 && dirY == 1)
        {
            controller.move(1 * stepWidth, 1 * stepWidth);

        }else if(dirX == 0 && dirY == -1)
        {
            controller.move(-1 * stepWidth, -1 * stepWidth);

        }else if (dirX == 1 && dirY == 0) 
        {
            controller.move(-1 * stepWidth, 1 * stepWidth);

        }else if (dirX == -1 && dirY == 0) 
        {
            controller.move(1 * stepWidth, -1 * stepWidth);

        }else if (dirX == 1 && dirY == 1) 
        {
            controller.move(0 * stepWidth, 2 * stepWidth);

        }else if (dirX == 1 && dirY == -1) 
        {
            controller.move(-2 * stepWidth, 0 * stepWidth);

        }else if (dirX == -1 && dirY == 1) 
        {
            controller.move(2 * stepWidth, 0 * stepWidth);

        }else if (dirX == -1 && dirY == -1) 
        {
            controller.move(0 * stepWidth, -2 * stepWidth);
        }


        disableSteppers();  
        
        currentX += dirX * stepWidth;
        currentY += dirY * stepWidth;

        occPositions[i][0] = currentX;
        occPositions[i][1] = currentY;

        
        /* code */
        // controller.move(1,1);
    }
}

bool positionOccupied(int x, int y, int occPositions[steps + 1][2]){
    for (int i = 0; i < steps + 1; i++)
    {
        if (x == occPositions[i][0] && y == occPositions[i][1])
        {
            Serial.println("position occupied");
            Serial.print(x);
            Serial.print(" / ");
            Serial.println(y);
            Serial.println("--");
            return true;
        }        
    }
    Serial.println("position free");
    Serial.print(x);
    Serial.print(" / ");
    Serial.println(y);
    Serial.println("-------------------");
    return false;
}

bool outOfCardBoundaries(int x, int y){
    if (x > boundaryRadius * 1.5 || x < -boundaryRadius * 1.5 || y > boundaryRadius || y < -boundaryRadius)
    {
        Serial.println("Position out of card boundaries:");
        Serial.print(x);
        Serial.print(" / ");
        Serial.println(y);
        return true;
    }
    return false;
    
}


// void testRun(){
//     int steps = 10;
//     for (int i = 0; i < steps; i++)
//     {
//         drawCircle();
//         step("up", steps);
//     }
//     drawCircle();

//     for (int i = 0; i < steps; i++)
//     {
//         drawLeaves();
//         step("down", steps);
//     }
//     drawLeaves();

//     for (int i = 0; i < steps / 2; i++)
//     {
//         step("right", steps);
//         step("up", 1);
//         step("right", steps);
//         step("down", 1);
//     }

//     step("left", 1);

//     for (int i = 0; i < steps / 2; i++)
//     {
//         step("left", steps);
//         step("up", 1);
//         step("left", steps);
//         step("down", 1);
//     }
//     step("right", 1);
    

// }

// void step(String dir, int stepWidth){
//     if (dir == "down")
//     {
//     controller.move(- quarter_rotation / stepWidth, -quarter_rotation / stepWidth);
//         /* code */
//     }else if(dir == "up"){
//     controller.move( quarter_rotation / stepWidth, quarter_rotation / stepWidth);

//     }
//     else if(dir == "left"){
//     controller.move( quarter_rotation / stepWidth, -quarter_rotation / stepWidth);

//     }
//     else if(dir == "right"){
//     controller.move( -quarter_rotation / stepWidth, quarter_rotation / stepWidth);

//     }
    
// }



// void drawLeaves(){
//     controller.move(-quarter_rotation, 0);
//     controller.move(quarter_rotation, 0);
//     controller.move(0, -quarter_rotation);
//     controller.move(0, quarter_rotation);
// }

// void drawCircle(){
//     controller.move(full_rotation / 5, -full_rotation / 5);
//     controller.move(-full_rotation / 5, full_rotation / 5);
//     controller.move(-full_rotation / 5, full_rotation / 5);
//     controller.move(full_rotation / 5, -full_rotation / 5);
// }


void enableSteppers(){
    // energize coils
    stepperR.enable();
    stepperL.enable();
}

void disableSteppers(){
    // pause and allow the motor to be moved by hand
    stepperL.disable();
    stepperR.disable();
}

