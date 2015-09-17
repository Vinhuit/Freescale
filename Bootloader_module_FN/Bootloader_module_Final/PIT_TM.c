#include "PIT_TM.h"
#include "LED.h"

void Pit_init(void)
{
		
		SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // enable PIT module
    PIT->MCR = 0x00;  // MDIS = 0  enables timer
		/* PIT1 */
		
    PIT->CHANNEL[1].LDVAL = 0x0000001E; // 
    PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TIE_MASK; // enable PIT1 and interrupt
		PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_CHN_MASK; // chain Timer 1 to Timer 0
    PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK; // Timer 1 is enabled
		/* PIT0 */
    PIT->CHANNEL[0].LDVAL =  0x00E42400; // 1s with FEQ 48MHz
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; /// Timer 0 is enabled  
	
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK; // enable PIT0 and interrupt

    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
		NVIC_EnableIRQ(PIT_IRQn);    /* Enable PIT Interrupt in NVIC*/  
}

