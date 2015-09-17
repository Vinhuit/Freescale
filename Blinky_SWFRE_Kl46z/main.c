/*-----------------------------------------
[GST2015]
Blinky Led 
Name : Huynh Ngoc Vinh
MS: 12520784

-------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h"
#define LED_User 2 												/* Led red */

extern void SystemCoreClockUpdate (void);
volatile uint32_t msTicks;  
const uint32_t led_mask[] = {1UL << 29,1UL << 5}; 	/*PTE 29 (Led red), PTD 5 (Led green)*/


/*Delay*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}
static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
/*Config LED*/
static void LED_Config(void) {

  SIM->SCGC5    |= (1UL << 13) | (1UL << 12);        		/* Enable Clock to Port E and Port D*/ 
  PORTE->PCR[29] = (1UL <<  8);        		/* Pin is GPIO Value MUX= 001 */
	PORTD->PCR[5]  = (1UL <<  8); 
  PTE->PDOR = led_mask[0];                /* Data Output  */
  PTD->PDOR = led_mask[1];  
	
  PTE->PDDR = led_mask[0];                /* enable Output Led red   */
	PTD->PDDR = led_mask[1];
                       
}


/*LED On*/
static void LED_On (uint32_t led) {
	if(led == 0) PTE->PCOR   = led_mask[led]; 						/*Set LOW Output */
	else 
		PTD->PCOR = led_mask[led];
}
/*LED Off*/
static void LED_Off (uint32_t led) {
  if(led == 0)PTE->PSOR   = led_mask[led]; 						/*Set LOW Output */
	else 
		PTD->PSOR = led_mask[led];
}
void Delay_ms(int value)
{uint32_t i,j;
	for(i=0;i<2500;i++)
	for(j=0;j<2500;j++)
	{}
}
int main(void){

	unsigned int c_LED = 0;
	
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  
	LED_Config(); 
	while(1){
	
		
		/* Blinky red with 250ms  and Green 500ms*/
		LED_On(c_LED);
		LED_On(c_LED+1);
		Delay_ms(250);
		LED_Off(c_LED);
		LED_Off(c_LED+1);
		Delay_ms(250);
		
	}
	
}