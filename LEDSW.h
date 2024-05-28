// LEDSW.h
// Runs on TM4C123 for CECS347 Project 2

#define LED (*((volatile unsigned long *)0x40025038))  // use onboard three LEDs: PF321
#define NVIC_EN0_PORTF 0x40000000
#define SW1 				0x10
#define SW2 				0x01

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// white    RGB    0x0E
// pink     R-B    0x06
// Cran     -GB    0x0C
#define Dark    	0x00
#define Red     	0x02
#define Blue    	0x04
#define Green   	0x08
#define Yellow  	0x0A
#define Cran      0x0C
#define White   	0x0E
#define Purple  	0x06

void LEDSW_Init(void);