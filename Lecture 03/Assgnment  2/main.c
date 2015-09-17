/*-----------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info: Implement the code to control LEDs by buttons. Press button to turn on LED; Release button to turn off LED
-------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h"
#define LED_User 1 												/* Led red */
#define SW1 0xfffffff7										/* Switch 1*/

uint32_t GT =0;
const unsigned char F = 0xff;
const unsigned char T = 0x00;
extern void SystemCoreClockUpdate (void);
volatile uint32_t msTicks;

const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTE 29 (Led red)*/
const uint32_t Button = (1UL << 3);  			/* SW1 */


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

  SIM->SCGC5    |= (1UL << 13)|(1UL << 12);        		/* Enable Clock to Port E */ 
  PORTE->PCR[29] = (1UL <<  8);        		/* Pin is GPIO Value = 0x000 */
	PORTD->PCR[5] = (1UL << 8);
  PTE->PDOR = led_mask[1];                /* Data Output  */
  PTD->PDOR = led_mask[0];
  
  PTD->PDDR = led_mask[0];                /* enable Output Led red   */
	PTE->PDDR = led_mask[1];
                       
}
static void BUTTON_Config(void) { 
	SIM->SCGC5    |= (1UL << 11);        		/* Enable Clock to Port C */ 
	PORTC->PCR[3] = (1UL << 8)|(1UL << 1)|(1UL << 0);	 						 /* GPIO with Internal pullup resistor*/
		
}

static int Get_Button(uint32_t key){

GT = PTC->PDIR;
if((GT|key)== key)
	return 1;
	return 0;
}
/*LED On*/
static void LED_On (uint32_t led) {
 if(led) PTE->PCOR   = led_mask[led]; 						/*Set LOW Output */
	else PTD->PCOR =led_mask[led];
}
/*LED Off*/
static void LED_Off (uint32_t led) {
  if(led)PTE->PSOR   = led_mask[led]; 						/*Set LOW Output */
	else
	PTD->PSOR   = led_mask[led]; 	
}
int main(void){
	
	unsigned char CB = 0x00;
	unsigned int c_LED = 0;
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  BUTTON_Config();
	LED_Config(); 
	while(1){
	
		if(Get_Button(SW1) == 1 )
		{	
			CB = ~ CB;
			Delay(500);
			if(CB == F){
						LED_On(c_LED);
						LED_On(c_LED+1);
				
				}
			else
				{
						LED_Off(c_LED);
						LED_Off(c_LED+1);
				}	
		
		}

	
	}
	
}