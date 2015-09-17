#include "LED.h"
unsigned int c_LED = 0;  									/* Choose Led :  0-Led green , 1-Ledred  */
/* Location LED */
const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTD5 (Led green),PTE29 (Led red)*/

void LED_Init(void) {

  SIM->SCGC5    |= (1UL << 13)|(1UL << 12);        		/* Enable Clock to Port E and Port D*/ 
  PORTE->PCR[29] = (1UL <<  8);        								/* Pin is GPIO Value MUX = 0x001 */
	PORTD->PCR[5] = (1UL << 8);
  PTE->PDOR |= led_mask[1];                						/* Data Output */
  PTD->PDOR |= led_mask[0];

  PTD->PDDR |= led_mask[0];                						/* Enable Output Led red  */
	PTE->PDDR |= led_mask[1];														/* Enable Output Led Green   */
                     
}
/*-------------------------------------------------------------------------------*/
/* Led ON */
/*-------------------------------------------------------------------------------*/
void LED_On (uint32_t led) {
 if(led) PTE->PCOR   |= led_mask[led]; 						/*Clear Output LED red */
	else PTD->PCOR |=led_mask[led];									/*Clear Output LED Green */
}
/*-------------------------------------------------------------------------------*/
/* Led Off */
/*-------------------------------------------------------------------------------*/
void LED_Off (uint32_t led) {
  if(led)PTE->PSOR   |= led_mask[led]; 						/*Set Output LED red */
	else PTD->PSOR   |= led_mask[led]; 							/*Set Output LED green*/
}
/*LED Toggle*/
void LED_Toggle(uint32_t led){
	if(led)PTE->PTOR   = led_mask[led]; 						/*Set LOW Output */
	else
	PTD->PTOR = led_mask[led]; 
};
