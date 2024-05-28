// Motors.h
// Runs on TM4C123 for CECS347 Project 2
#define WHEEL_DIR (*((volatile unsigned long *)0x40005330)) // PB7632 are the four direction pins for L298
	
// Constant definitions based on the following hardware interface:
// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
#define FORWARD 0x88    //PB7&PB3
#define BACKWARD 0x44   //PB6&PB2
#define LEFTPIVOT 0x48  //PB6&PB3
#define RIGHTPIVOT 0x84 //PB7&PB2

// Constant definitions based on the following hardware interface:
// System clock is 16MHz
#define PERIOD     16000
#define LEFTPOWER  0.43*PERIOD // with equal power to both motors (LeftH == RightH), the robot still may not drive straight
#define RIGHTPOWER 0.43*PERIOD // mechanical differences in the motor, left wheel faster or slower than the constant right wheel
#define NOPOWER       0*PERIOD // no duty cycle for deactivation purpose

// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
//	Initializes the PWM module 1 signals tied to PF321 on the Tiva Launchpad 
//		to allow for changing brightness of LEDs based on vehicle speed.
//	Initializes the PWM module 0 signals tied to PB54 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB54_Init(void);

// Dependency: PWM_PB54_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB54 by changing the CMP registers
void PWM_PB54_Duty(unsigned long duty_L, unsigned long duty_R);
/////////////////////////////////////////////////////////////////////////////

void Motors_Init(void);