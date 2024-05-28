// Motors.c
// Runs on TM4C123 for CECS347 Project 2
#include "tm4c123gh6pm.h"
#include "Motors.h"

// Dependency: None
// Inputs: None
// Outputs: None
// Description: Initializes PB7632 for use with L298N motor driver direction
void Motors_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
  GPIO_PORTB_AMSEL_R &= ~0xCC;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~0xCC;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0xFF00FF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0xCC; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0xCC;	// enable digital I/O on pin(s)
}

//////////////////////3. Subroutines Section/////////////////////////////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
// Initializes the PWM module 0 generator 0 outputs A&B tied to PB54 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB54_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
	
	GPIO_PORTB_AFSEL_R |= 0x30;	      // enable alt funct: PB54 for PWM
  GPIO_PORTB_PCTL_R &= ~0x00FF0000; // PWM to be used
  GPIO_PORTB_PCTL_R |= 0x00440000;  // PWM to be used //use table 35
  GPIO_PORTB_DEN_R |= 0x30;	        // enable digital I/O 
	
	// Initializes PWM settings
	SYSCTL_RCGCPWM_R |= 0x01;	   // activate PWM0
	SYSCTL_RCC_R &= ~0x001E0000; // Clear any previous PWM divider values
	
	// PWM0_0 output A&B Initialization for PB54
	PWM0_1_CTL_R = 0;	                // re-loading down-counting mode
	PWM0_1_GENA_R |= 0xC8;	          // low on LOAD, high on CMPA down
	PWM0_1_GENB_R |= 0xC08;           // low on LOAD, high on CMPB down
	PWM0_1_LOAD_R = PERIOD - 1;	// cycles needed to count down to 0
  PWM0_1_CMPA_R = 0;	              // count value when output rises //50% cycle
	PWM0_1_CMPB_R = 0;	              // count value when output rises
	
	PWM0_1_CTL_R |= 0x00000001;	  // Enable PWM0 Generator 0 in Countdown mode
	PWM0_ENABLE_R &= ~0x0000000C;	// Disable PB54:PWM0 output 0&1 on initialization // page 41
}

// Dependency: PWM_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB54 by changing the CMP registers
void PWM_PB54_Duty(unsigned long duty_L, unsigned long duty_R){
	PWM0_1_CMPA_R = duty_L - 1;	// PB4 count value when output rises
  PWM0_1_CMPB_R = duty_R - 1;	// PB5 count value when output rises
}
/////////////////////////////////////////////////////////////////////////////