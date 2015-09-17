#include "MKL46Z4.h" 
#include "BL_DrvTimer.h"
int Timer0_LVAL=0x01AAA2BF;											//27959999
int Timer1_LVAL=0X0355457F;											//55919999
/*----------------------------------------------------------------------------
Timer Config
*----------------------------------------------------------------------------*/
void TimerConfig(void)
{
	//Turn on Clock PIT 
	SIM->SCGC6 |= 1UL<<23;	 
	// Turn on PIT
	PIT->MCR &= 0x00UL;
	// Timer 0
	PIT->CHANNEL[0].LDVAL  = Timer0_LVAL; 				// setup Timer0 for Timer0_LVAL cycles
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; 	// enable Timer 0 interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; 	// enable Timer 0
	// Timer 1
	PIT->CHANNEL[1].LDVAL  = Timer1_LVAL; 				// setup Timer 1 for Timer1_LVAL cycles
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK; 	// enable Timer 1 interrupts
	//PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK; 	// enable Timer 1
		/*Set priority for PIT interrupt*/
	NVIC_SetPriority(PIT_IRQn,2);
	/*Turn on PIT interrupt*/
	NVIC_EnableIRQ(PIT_IRQn);
  
}

