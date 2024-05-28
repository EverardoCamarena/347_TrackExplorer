// LEDSW.c
// Runs on TM4C123 for CECS347 Project 2
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Motors.h"
#include "LEDSW.h"

// Port F Initialization + Swich 1 & 2
void LEDSW_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
	
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // 1) unlock GPIO Port F
	GPIO_PORTF_CR_R |= 0x1F;         	 // 2) allow changes to PF0-4
	GPIO_PORTF_AMSEL_R &= ~0x1F;       // 3) disable analog function
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF;  // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~0x11;         // 5) PF4&PF0 input  
	GPIO_PORTF_DIR_R |= 0x0E;          // 6) PF1-PF3 output
  GPIO_PORTF_AFSEL_R &= ~0x1F;       // 7) no alternate function     
  GPIO_PORTF_PUR_R |= 0x11;          // 8) enable pullup resistors on PF4,PF0
	GPIO_PORTF_DEN_R |= 0x1F;          // 9) enable digital pins PF3-PF1
  LED = Dark;                        // 10) Turn off all LEDs.
		
  GPIO_PORTF_IS_R &= ~0x11;          // 11) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;         // 12) PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R |= ~0x11;         // 13) PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R |= 0x11;          // 14) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;           // 15) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // 16) bits:23-21 for PORTF, set priority to 2
  NVIC_EN0_R = NVIC_EN0_PORTF;       // 17) enable interrupt 30 in NVIC
}

// PORTF ISR:
// Change delivered power based on switch press: 
// sw1: activate the robot
// sw2: deactivate the robot
void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
	// simple debouncing code: generate 20ms to 30ms delay
	for (uint32_t time=0;time<72724;time++) {}
		
  if(GPIO_PORTF_RIS_R & SW1){  // SW1 touched
    GPIO_PORTF_ICR_R = SW1;    // acknowledge flag0
		// activate
		PWM_PB54_Duty(LEFTPOWER, RIGHTPOWER); // Initiate PWM duty cycle to given 36%
  }
	
  if(GPIO_PORTF_RIS_R & SW2){  // SW2 touched 
    GPIO_PORTF_ICR_R = SW2;    // acknowledge flag4
		// deactivate
		PWM_PB54_Duty(NOPOWER, NOPOWER); // Update PWM duty cycle to 0%
  }
}