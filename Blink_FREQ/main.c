/*-------------------------------------------------------------------------------
[GST2015]
Name : Huynh Ngoc Vinh
MS: 12520784
Info: 2.	Using SysTick for RED LED blinks at 1Hz; Green LED blink at 2 Hz
-------------------------------------------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h" /* Device header*/

volatile uint32_t msTicks; /* count 1ms timerStick */
extern void SystemCoreClockUpdate (void);
const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTE 29 (Led red), PTD5 (led Green)*/


/*-----------------------------------------------------*/
/* Delay */
/*-----------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;  								  /* increment counter necessary in Delay() */
}
/*Delay number of tick stick (1ms)*/
static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
/*---------------------------------------------------------------------------------*/
/*Config LED*/
/*--------------------5-------------------------------------------------------------*/
static void LED_Init(void) {

  SIM->SCGC5    |= (1UL << 13)|(1UL << 12);        		/* Enable Clock to Port E and Port D*/ 
  PORTE->PCR[29] = (1UL <<  8);        								/* Pin is GPIO Value MUX = 0x001 */
	PORTD->PCR[5] = (1UL << 8);
  PTE->PDOR = led_mask[1];                						/* Data Output */
  PTD->PDOR = led_mask[0];
  
  PTD->PDDR = led_mask[0];                						/* Enable Output Led red  */
	PTE->PDDR = led_mask[1];														/* Enable Output Led Green   */
                       
}
/*-------------------------------------------------------------------------------*/
/* Led ON */
/*-------------------------------------------------------------------------------*/
static void LED_On (uint32_t led) {
 if(led) PTE->PCOR   = led_mask[led]; 						/*Clear Output LED red */
	else PTD->PCOR =led_mask[led];									/*Clear Output LED Green */
}
/*-------------------------------------------------------------------------------*/
/* Led Off */
/*-------------------------------------------------------------------------------*/
static void LED_Off (uint32_t led) {
  if(led)PTE->PSOR   = led_mask[led]; 						/*Set Output LED red */
	else PTD->PSOR   = led_mask[led]; 							/*Set Output LED green*/
}
/*-------------------------------------------------------------------------------*/
int main(void)
{
	uint8_t c_LED = 0; 														/* 0 :Led RED -- 1:Led Green */
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms */
	LED_Init();																		/* Init LEd */
		
	while(1)
	{	
					
		/* Blinky red with 1Hz  and Green 2Hz */
		LED_On(c_LED+1); 							/* Led RED  */
		LED_On(c_LED); 								/* Led Green */
		Delay(250);										/* Delay 250 ms*/
		
		LED_Off(c_LED+1);
		Delay(250);
		
		LED_Off(c_LED);
		LED_On(c_LED+1); 		
		Delay(250);										
		
		LED_Off(c_LED+1);
		Delay(250);
		
	}

}

