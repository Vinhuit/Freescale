#include "MKL46Z4.h"  
#include "BL_DrvLed.h"

const uint32_t led_mask[] = {1UL << 5, 1UL << 29};
/*------------------------------------------------------------------------------
  Configure LED pins
 *------------------------------------------------------------------------------*/
void LED_Config(void) {

  SIM->SCGC5    |= (1UL << 12) | (1UL << 13);        /* Enable Clock to Port D and Port E*/ 
  PORTD->PCR[5]  = (1UL <<  8);                      /* Pin is GPIO */
  PORTE->PCR[29] = (1UL <<  8);                      /* Pin is GPIO */

  PTD->PDOR = led_mask[0];                           /* switch LEDs off  */
  PTE->PDOR = led_mask[1];                       

  PTD->PDDR = led_mask[0];                           /* enable Output  */
  PTE->PDDR = led_mask[1];                       
}

/*------------------------------------------------------------------------------
  Switch on LEDs
 *------------------------------------------------------------------------------*/
void LED_On (uint32_t led) {
  if (led) PTE->PCOR   = led_mask[led];	//PTE mau do
  else PTD->PCOR       = led_mask[led];
}

/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/
void LED_Off (uint32_t led) {
  if (led) PTE->PSOR   = led_mask[led];
  else PTD->PSOR       = led_mask[led];
}
/*------------------------------------------------------------------------------
  TOGGLE  LEDs
 *------------------------------------------------------------------------------*/
void TOGGLE (uint32_t led){
	if(led) PTE -> PTOR = led_mask[led];
	else PTD-> PTOR = led_mask[led];
}

