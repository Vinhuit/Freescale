#include  <stdio.h>
#include  <MKL46Z4.h> 										/*Device Header*/
#include "Button.h"


void Button_init(void) 											 
{ 
	SIM->SCGC5    |= (1UL << 11);        							/* Enable Clock to Port C */ 
	PORTC->PCR[3] = (1UL << 8)|(1UL << 1)|(1UL << 0)|(1UL << 19)|(1UL << 17);	 	/* GPIO PTC3 with Internal pullup resistor and Interrupt*/
	PORTC->PCR[12] = (1UL << 8)|(1UL << 1)|(1UL << 0)|(1UL << 19)|(1UL << 17);	/* GPIO PTC12 with Internal pullup resistor and Interrupt*/
	NVIC_EnableIRQ(PORTC_PORTD_IRQn); /* Enable Vector Interrupt in Port C and Port D */
}
/*-------------------------------------------------------------------------------*/
/* Get Button to user*/
/*-------------------------------------------------------------------------------*/
int Get_Button(uint32_t key) 
{
uint32_t GT;
GT = PTC->PDIR;  /* Data Input Register PORTC */

if((GT|key)== key) /* Check changes Data PORTC */
	return 1;
	return 0;
}


