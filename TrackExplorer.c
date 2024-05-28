// TrackExplorer.c
// Runs on TM4C123
// This is the starter file for CECS 347 Project 2 - A Track Explorer
// This project uses hardware PWM to control two DC Motors, 
// ADC to collect analog inputs from three Sharp IR sensors.
// The three Sharp analog IR distance sensors (GP2Y0A21YK0F) are used
// to allowthe robot to navigate through a track with two walls: 
// one mounted looking directly forward to avoid a head-on collision, 
// the other two looking forward to the left and to the right to detect  
// the distances between the car and the two walls. The goal is to 
// control power to each wheel so the left and right distances to the 
// walls are equal.
// If an object is detected too close to the robot, 
// the robot should be able to avoid it.
/*
    ------------------------------------------wall---------
                      /
                     /
                    / 
                   /
         -----------
         |         |
         | Robot   | ---> direction of motion and third sensor
         |         |
         -----------
                   \
                    \
                     \
                      \
    ------------------------------------------wall---------
*/
// The original project is designed by Dr. Daniel Valvano, Jonathan Valvano
// September 12, 2013
// Modifications are made by Dr. Min He.

// PE1 connected to left facing IR distance sensor
// PE4 connected to front IR distance sensor
// PE5 connected to right IR distance sensor

#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "Sensors.h"
#include "Motors.h"
#include "LEDSW.h"
#include "PLL.h"

// basic functions defined at end of startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// use the datasheet to calculate ADC values
// then test your sensors to adjust said values 
// change measurements based on our sensors
#define CRASH             IR15CM // if there is less than this distance ahead of the robot, it will immediately stop
#define IR15CM            2190   // ADC output for 15cm:1.8v -> (1.8/3.3)*4095=2233
#define IR20CM            1845
#define IR25CM            1500   // ADC output for 20cm:1.39v -> (1.39/3.3)*4095=1724
#define IR35CM            1078   // ADC output for 30cm:0.9v -> (0.9/3.3)*4095=1116
#define IR45CM            926    // ADC output for 40cm:0.74v -> (0.74/3.3)*4095=918
#define IR70CM            453    // ADC output for 80cm:0.4v -> (0.4/3.3)*4095=496								

void System_Init(void);
void steering(uint16_t ahead_dist,uint16_t right_dist, uint16_t left_dist);

int main(void){
  uint16_t left, right, ahead;
  
  DisableInterrupts(); // disable interrupts while initializing
  System_Init();
  EnableInterrupts();  // enable after all initialization are done
	
	
	// wait until robot is activated i.e SW1 is pressed
	while((SW1 & 0x10) == 0){}
	
  // read sensors 10 times before the car starts to move
	// this will allow software to filter the sensor outputs	
	for(uint8_t i = 0; i < 10; i++){
		ReadSensorsFIRFilter(&ahead, &right, &left);
	}

	// move forward after sensors are calibrated
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= 0x0000000C; // enable both wheels

  while(1){
		// choose one of the following three software filter methods
		ReadSensorsMedianFilter(&ahead, &right, &left);
//		ReadSensorsIIRFilter(&ahead, &right, &left);
//		ReadSensorsFIRFilter(&ahead, &right, &left);
		
		steering(ahead,right,left); 
  }
}

void System_Init(void) {
  PLL_Init();           // bus clock at 16 MHz
  Sensors_Init();       // initialize ADC to sample AIN2 (PE1), AIN9 (PE4), AIN8 (PE5)
  LEDSW_Init();         // configure onboard LEDs and push buttons
  Motors_Init();        // Initialize signals for the two DC Motors
	PWM_PB54_Init();      // Initializes the PWM module 0 generator 0 outputs A&B tied to PB54
}

void steering(uint16_t ahead_dist, uint16_t right_dist, uint16_t left_dist){
	// Too close to the left wall go back to the center
	if (left_dist >= IR25CM) {
    LED = Red;
    WHEEL_DIR = FORWARD;          // Turn right
    PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
    PWM0_ENABLE_R |= 0x00000004;  // Enable left wheel
  }

	// Too close to the right wall go back to the center
  if (right_dist >= IR25CM) {
    LED = Red;
    WHEEL_DIR = FORWARD;          // Turn left
    PWM0_ENABLE_R |= 0x00000008;  // Enable right wheel
    PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
  }

	// Go straight if the car is within centered distance
  if (left_dist <= IR25CM && ahead_dist <= IR25CM && right_dist <= IR25CM) {
    WHEEL_DIR = FORWARD;
    PWM0_ENABLE_R |= 0x0000000C;  // enable both wheels
		LED = Green;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Pivot right
  if (ahead_dist >= IR25CM && right_dist >= IR70CM) {
		LED = Red;
		WHEEL_DIR = RIGHTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
  }
	
	// Pivot left
	if (ahead_dist >= IR25CM && left_dist >= IR70CM) {
    LED = Red;
		WHEEL_DIR = LEFTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
  }
	
	// Left pivot turn when there are obstacles in all 3 directions
	if (left_dist >= IR25CM && ahead_dist >= IR25CM && right_dist >= IR25CM){
		LED = Red;
		WHEEL_DIR = LEFTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
	}
	
	// We came back to the beginnig and no obstacles are in sight
	if(left_dist <= IR70CM && ahead_dist <= IR70CM && right_dist <= IR70CM){
		LED = Blue;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	}
}
