/*------------------------------------------
UIT - CE07
Tong Tri Kien - 12520210

Using for control SW1 and SW3 in FRDM-KL46Z
-------------------------------------------*/

#include "MKL46Z4.h"
#include "DrvSW.h"

/******************************************************************/
//						Function for Initialization of SW									//
/******************************************************************/

/*-----------------------------
Configure SW pins
-----------------------------*/
void DrvSW_Config(void)
{
	SIM->SCGC5    |= (1UL << 11 );
	PORTC->PCR[3] = (1UL << 0 | 1UL << 1 | 1UL <<  8);
	PORTC->PCR[12] = (1UL << 0 | 1UL << 1 | 1UL <<  8);
}

/*-----------------------------
Configure SW pins with Interrupt
-----------------------------*/
void DrvSW_Interrupt_Config(void)
{
	SIM->SCGC5    |= (1UL << 11 );
	PORTC->PCR[3] = (1UL << 0 | 1UL << 1 | 1UL << 8 | 1UL << 17 | 1UL << 19 | 1UL << 24);
	PORTC->PCR[12] = (1UL << 0 | 1UL << 1 | 1UL << 8 | 1UL << 17 | 1UL << 19 | 1UL << 24);
	
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	
}

/*-----------------------------
Get SW
-----------------------------*/
uint32_t DrvSW_Get(void)
{
	uint32_t sw;
	sw = PTC->PDIR;
	if ((sw | 0xFFFFFFF7) == 0xFFFFFFF7) return 1; // Press SW1
	else if ((sw | 0xFFFFEFFF) == 0xFFFFEFFF) return 3; // Press SW3
	else return 0;
}
