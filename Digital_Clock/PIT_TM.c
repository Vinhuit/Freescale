#include "PIT_TM.h"


void Pit_init(void)
{

		SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // enable PIT module
    PIT->MCR = 0x00;  // MDIS = 0  enables timer
		/* PIT0 */
    PIT->CHANNEL[0].TCTRL = 0x00; // disable PIT0
    PIT->CHANNEL[0].LDVAL = 0x00D55160; // 
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK; // enable PIT0 and interrupt
    PIT->CHANNEL[0].TFLG = 0x01; // clear flag
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
		
		NVIC_EnableIRQ(PIT_IRQn);    /* Enable PIT Interrupt in NVIC*/  
}

