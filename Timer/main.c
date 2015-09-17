/*-------------------------------------------------------------------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info: the code to trigger the PIT timer interrupt on 10 seconds with chaining both channel 0 & 1

----------------------------------------------------------------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h" 										/*Device Header*/

#define LED_User 2												/* Led used */

unsigned int c_LED = 0;  									/* Choose Led :  0-Led green , 1-Ledred  */
/* Location LED */
const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTD5 (Led green),PTE29 (Led red)*/

/*---------------------------------------------------------------------------------*/
/*Config LED*/
/*---------------------------------------------------------------------------------*/
static void LED_Init(void) {

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
static void LED_On (uint32_t led) {
 if(led) PTE->PCOR   |= led_mask[led]; 						/*Clear Output LED red */
	else PTD->PCOR |=led_mask[led];									/*Clear Output LED Green */
}
///*-------------------------------------------------------------------------------*/
///* Led Off */
///*-------------------------------------------------------------------------------*/
//static void LED_Off (uint32_t led) {
//  if(led)PTE->PSOR   |= led_mask[led]; 						/*Set Output LED red */
//	else PTD->PSOR   |= led_mask[led]; 							/*Set Output LED green*/
//}
/*LED Toggle*/
static void LED_Toggle(uint32_t led){
	if(led)PTE->PTOR   = led_mask[led]; 						/*Set LOW Output */
	else
	PTD->PTOR = led_mask[led]; 
}

void Pit_init(void)
{

		SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // enable PIT module
    PIT->MCR = 0x00;  // MDIS = 0  enables timer
		/* PIT1 */
		
    PIT->CHANNEL[1].LDVAL = 0x00000000; // 60s*10 = 1min
    PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TIE_MASK; // enable PIT1 and interrupt
		PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_CHN_MASK; // chain Timer 1 to Timer 0
    PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK; // Timer 1 is enabled
		/* PIT0 */
    PIT->CHANNEL[0].LDVAL = 0x007A1200; // 60s with FEQ 8MHz
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; /// Timer 0 is enabled
		NVIC_EnableIRQ(PIT_IRQn);    /* Enable PIT Interrupt in NVIC*/  
		
}
void PIT_IRQHandler(void)
{ 
	/* every 10s toggle LED */
	/* Interrupt PIT1 */
	if(PIT->CHANNEL[1].TFLG == 1)
	{
		LED_Toggle(c_LED+1);					//LED red toggle
		LED_Toggle(c_LED);
		PIT->CHANNEL[1].TFLG =0x01; // clear flag
	}
}

void FBE(void)
	{
	
		MCG->C6 &= ~MCG_C6_CME0_MASK; 					//External clock monitor is disabled for OSC0.

    MCG->C2 |= MCG_C2_RANGE0(3) |						// Very high frequency range selected for the crystal oscillator 
               MCG_C2_EREFS0_MASK  ; 				//Oscillator requested
		
    MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;   // Reset DCO Range
    MCG->C4 &= ~MCG_C4_DMX32_MASK;			// DCO Maximum Frequency
    MCG->C4 |= MCG_C4_DRST_DRS(1);			// 31.25 * 1280 = 40000kHz
    
    MCG->C6 &= ~MCG_C6_PLLS_MASK;				// Select FLL
			
    MCG->C1 &= ~MCG_C1_CLKS_MASK;				// Reset Clock Source Select
    MCG->C1 |= MCG_C1_CLKS(2) | 				//External reference clock is selected
						 	MCG_C1_FRDIV(3)| 					// Divide Factor is 256
							MCG_C1_IRCLKEN_MASK;			//MCGIRCLK active
					// Output of FLL is selected for MCGOUTCLK
    
    while((MCG->S & MCG_S_OSCINIT0_MASK) == 0); 		// wait for osc init
    while((MCG->S & MCG_S_PLLST_MASK) != 0); 				// wait for FLL
    while((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2)); // wait for EXTAL is selected
		 SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(1 - 1) |  				// core/system clock = MCGOUTCLK / 1 = 8 / 1 = 8MHz
     SIM_CLKDIV1_OUTDIV4(1 - 1);   // flash/bus clock = core/system / 1 = 8MHz
	}


int main(void){
	
	LED_Init();  //init LED
	Pit_init();	//init PIT timer 
	FBE();			// FEI to FBE Bus clock = 8MHZ
	/* Turn on 2 LED*/
	LED_On(c_LED);		
	LED_On(c_LED+1);

 	while(1)
	{
		
		/* With MCG_FBE confirm Clock BUS = 8MHZ 
		the code to trigger the PIT timer interrupt on 10 seconds with chaining both channel 0 & 1
		 Blinky Led duty cycle 10s .
		*/
	}

}
	
