/*------------------------------------------
UIT - CE07
Tong Tri Kien - 12520210

Using for control LED in FRDM-KL46Z
-------------------------------------------*/

#include "MKL46Z4.h"
#include "DrvLED.h"

const static uint32_t led_mask[] = {1UL << 5, 1UL << 29};

/******************************************************************/
//						Function for Initialization of LED									//
/******************************************************************/

/*-----------------------------
Configure LED pins
-----------------------------*/
void DrvLED_Config(void)
{

  SIM->SCGC5    |= (1UL << 12) | (1UL << 13);        /* Enable Clock to Port D and Port E*/ 
  PORTD->PCR[5]  = (1UL <<  8);                      /* Pin is GPIO */
  PORTE->PCR[29] = (1UL <<  8);                      /* Pin is GPIO */

  PTD->PDOR = led_mask[0];                           /* switch LEDs off  */
  PTE->PDOR = led_mask[1];                       

  PTD->PDDR = led_mask[0];                           /* enable Output  */
  PTE->PDDR = led_mask[1];                       
}

/*------------------------------
  Switch on LEDs
------------------------------*/
void DrvLED_On (uint32_t led)
{
  if (led) PTE->PCOR   = led_mask[led];
  else PTD->PCOR       = led_mask[led];
}

/*------------------------------------------------------------------------------
  Switch off LEDs
 *------------------------------------------------------------------------------*/ 
void DrvLED_Off (uint32_t led)
{
  if (led) PTE->PSOR   = led_mask[led];
	else PTD->PSOR       = led_mask[led];
}

/*------------------------------------------------------------------------------
  Toggle LEDs
 *------------------------------------------------------------------------------*/
void DrvLED_Toggle(uint32_t led)
{
	if (led) PTE->PTOR   = led_mask[led];
	else PTD->PTOR       = led_mask[led];
}
