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
#define RPM 60

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

const float full_rotation_R = MOTOR_STEPS * MICROSTEPS * 2.5;
const float full_rotation_L = MOTOR_STEPS * MICROSTEPS * 7;

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
    Serial.println("Test Sketch");


     // energize coils
    stepperR.enable();
    stepperL.enable();
    // drawSpiral(10);

 


    for (float i = 0; i < full_rotation_L; i+= full_rotation_L / 12)
    {
        

        for (float r = 0; r < full_rotation_R / 2; r+= full_rotation_R / 32 )
        {
            drawSomething();
            controller.move(0, full_rotation_R / 32);
            
        }
        controller.move(0, -full_rotation_R / 2);

        controller.move(full_rotation_L / 12, 0);

    } 
    

   



    // pause and allow the motor to be moved by hand
    stepperL.disable();
    stepperR.disable();
}

void drawXTimes(int x){
    // energize coils
  stepperR.enable();
  stepperL.enable();

 
  
  // Moving motor one full revolution using the degree notation

  stepperR.rotate(full_rotation_R / 10);
  stepperR.rotate(- full_rotation_R / 10);

  //stepperL.move(-MOTOR_STEPS*MICROSTEPS*7); // 200*7 = 1400 steps to rotate the entire disk
  //stepperR.move(-MOTOR_STEPS * MICROSTEPS * 2.5); // 200*7 = 1400 steps to rotate the entire disk
  
  // pause and allow the motor to be moved by hand
  stepperL.disable();
  stepperR.disable();

//   delay(2000); // repeat after 2sec. pause
//   drawXTimes(1);
}

void loop(){
}

void drawSpiral(int rotations){


    int angle = rotations * full_rotation_L;
    controller.move(angle, full_rotation_R / 3);


}

void drawSomething(){
    controller.move(full_rotation_L / 48, full_rotation_R / 20);
    controller.move(full_rotation_L / 48, -full_rotation_R / 20);
    controller.move(-full_rotation_L / 48, -full_rotation_R / 20);
    controller.move(-full_rotation_L / 48, full_rotation_R / 20);
}


