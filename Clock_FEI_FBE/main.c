/*-----------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info: Implement the code to transit the KL46 from FEI mode to FBE when Get SW1
"	Configure MCUOUTCLK = Bus Clock = 8MHz
"	Configure FLL Output = 40MHz
"	Configure CLKOUT=PTC3=Bus Clock = 8MHz
------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h" 										/*Device Header*/
#define LED_User 2												/* Led used */
#define BT1 0xfffffff7										/* Switch 1*/
#define BT2 0xffffefff										/* Switch 2*/

unsigned int  t = 250; 										/* variable Delay for LED Blinky */
unsigned char CB = 1; 									/* toggle */
unsigned int c_LED = 0;  									/* Choose Led :  0-Led red , 1-Ledgreen  */
/* Statup */
const char F = 0xff;  										/* Statup False   */
const char T = 0x00;											/* Statup True  */
extern void SystemCoreClockUpdate (void);

/* count 1ms timerStick */
volatile uint32_t msTicks;  

/* Location LED */
const uint32_t led_mask[] = {1UL << 5,1UL << 29}; 	/*PTE29 (Led red),PTD5 (Led green)*/

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
void FEI_to_FBE (void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;  	// enable clock for port A
	
	MCG->C2 |= MCG_C2_RANGE0(0x1) ;				// Very high frequency range selected for the crystal oscillator 
  MCG->C2 &= ~MCG_C2_HGO0_MASK ; 				// Configure crystal oscillator for low-power operation.
	MCG->C2 |= MCG_C2_EREFS0_MASK ;				// Select Oscillator requested.
	
	MCG->C6 &=~MCG_C6_PLLS_MASK ; 				// FLL is selected
	MCG->C2 &=~MCG_C2_LP_MASK   ; 				// FLL or PLL is not disabled in bypass modes .
	
	MCG->C1 |= MCG_C1_CLKS(0x2);  				// External reference clock is selected.
	MCG->C1 &=~MCG_C1_IREFS_MASK; 				// Selects the reference clock source for the FLL. External reference clock is selected.
	MCG->C1 |=MCG_C1_FRDIV(0x3) ;   			// Divide Factor is 256, OSC clock = 8MHz => 8000 / 256 = 31.25kHz
	MCG->C1 |=MCG_C1_IRCLKEN_MASK ;				// Enable Internal reference clock as MCGIRCLK
	
	MCG->C4 &= ~MCG_C4_DMX32_MASK;  			// FLL Factor =1280 
  MCG->C4 |= MCG_C4_DRST_DRS(1); 				// Reference range in 31.25–39.0625 kHz => FLL output=31.25 * 1280 = 40MHz
	OSC0->CR = (uint8_t)0x80U;
	
	/*Waiting for everything is config*/
	while((MCG->S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
  }
  while((MCG->S & 0x0CU) != 0x08U) {    					/* Wait until external reference clock is selected as MCG output , MCG_S_CLKST*/
  }
	
	
}
void FBE_to_PBE (void )
{
	MCG->C5 |=MCG_C5_PRDIV0(0x3); 			// Divider Factor = 4 , 8MHz/4=2MHz in range 2MHz->4MHz	
	MCG->C6 |=MCG_C6_PLLS_MASK ; 				// PLL is selected
	MCG->C6 |=MCG_C6_VDIV0(0x18);       // PLL out = 2MHz * 48 = 96MHz
	
	while((MCG->S & 0x0CU) != 0x08U) {    				/* Wait until external reference clock is selected as MCG output */
  }
  while((MCG->S & MCG_S_LOCK0_MASK) == 0x00U) { /* Wait until locked */
  }

}
void PBE_to_PEE(void)
{
	MCG->C5 |= MCG_C5_PLLCLKEN0_MASK  ;				// MCGPLLCLK is active
	SIM->SOPT2 |=SIM_SOPT2_PLLFLLSEL_MASK; 		// Selects the MCGPLLCLK clock for various peripheral clocking options
	MCG->C1 &=~MCG_C1_CLKS_MASK;  					 	// PLL or FLL reference clock is selected.
	
	SIM->CLKDIV1 |=SIM_CLKDIV1_OUTDIV1(0x1);  //Factor Divider1 == 2 => core clock = 96MHz/2 =48MHz 
	SIM->CLKDIV1 |=SIM_CLKDIV1_OUTDIV4(0x1); 	//Factor Divider4 == 2 => bus clock  = 48MHz/2 =24MHz 
	
	while((MCG->S & 0x0CU) != 0x0CU) {     		/* Wait until output of the PLL is selected */
  }

}
/*-------------------------------------------------------------------------------*/
/* Led ON */
/*-------------------------------------------------------------------------------*/
static void LED_On (uint32_t led) {
 if(led) PTE->PCOR   |= led_mask[led]; 						/*Clear Output LED red */
	else PTD->PCOR |=led_mask[led];									/*Clear Output LED Green */
}
/*-------------------------------------------------------------------------------*/
/* Led Off */
/*-------------------------------------------------------------------------------*/
static void LED_Off (uint32_t led) {
  if(led)PTE->PSOR   |= led_mask[led]; 						/*Set Output LED red */
	else PTD->PSOR   |= led_mask[led]; 							/*Set Output LED green*/
}
/*-------------------------------------------------------------------------------*/
/*	Init Button */
/*-------------------------------------------------------------------------------*/
static void Button_init(void) 											 
{ 
	SIM->SCGC5    |= (1UL << 11);        							/* Enable Clock to Port C */ 
	PORTC->PCR[3] = (1UL << 8)|(1UL << 1)|(1UL << 0)|(1UL << 19)|(1UL << 17);	 	/* GPIO PTC3 with Internal pullup resistor and Interrupt*/
	PORTC->PCR[12] = (1UL << 8)|(1UL << 1)|(1UL << 0)|(1UL << 19)|(1UL << 17);	/* GPIO PTC12 with Internal pullup resistor and Interrupt*/
	NVIC_EnableIRQ(PORTC_PORTD_IRQn); /* Enable Vector Interrupt in Port C and Port D */
}
/*-------------------------------------------------------------------------------*/
/* Get Button to user*/
/*-------------------------------------------------------------------------------*/
static int Get_Button(uint32_t key) 
{
uint32_t GT;
GT = PTC->PDIR;  /* Data Input Register PORTC */

if((GT|key)== key) /* Check changes Data PORTC */
	return 1;
	return 0;
}

void SysTick_Handler(void) {
	
  msTicks++;												/* increment counter necessary in Delay() */		
}
/*-----------------------------------------------------*/
/* Delay */
/*-----------------------------------------------------*/
/*Delay number of tick stick (1ms)*/
static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
/*-------------------------------------------------------------------------------*/
/* switch to FBE 
	Configure MCUOUTCLK = Bus Clock = 8MHz
"	Configure FLL Output = 40MHz
"	Configure CLKOUT=PTC3=Bus Clock = 8MHz */
/*-------------------------------------------------------------------------------*/
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
/*-------------------------------------------------------------------------------*/
/* switch to FEI 24MHz External crystal */
/*-------------------------------------------------------------------------------*/

	// switch to PBE 8MHz External crystal
/* Switch to PBE Mode */
void PBE(void)
{
	  MCG->C6 &= ~MCG_C6_CME0_MASK;

    MCG->C2 &= ~MCG_C2_LP_MASK;
    MCG->C2 |= MCG_C2_RANGE0(3) |// Very high frequency range selected for the crystal oscillator 
               MCG_C2_EREFS0_MASK;

    MCG->C5 &= ~MCG_C5_PRDIV0_MASK;
    MCG->C5 |= MCG_C5_PRDIV0(2 - 1); // External clock div 2
    
    MCG->C6 &= ~MCG_C6_VDIV0_MASK;
    MCG->C6 |= MCG_C6_VDIV0(24 - 24) | // Mul 24. 8 / 2 * 24 = 96MHz
               MCG_C6_CME0_MASK |
               MCG_C6_PLLS_MASK;

    MCG->C1 &= ~MCG_C1_CLKS_MASK;
    MCG->C1 |= MCG_C1_CLKS(2); // Output of ExTAL is selected for MCGOUTCLK
    
    while((MCG->S & MCG_S_OSCINIT0_MASK) == 0); // wait for osc init.
    while((MCG->S & MCG_S_PLLST_MASK) != MCG_S_PLLST_MASK); // wait for PLL
    while((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2)); // wait for EXTAL is selected
}

void PEE(void)
{
	  MCG->C6 &= ~MCG_C6_CME0_MASK;

    MCG->C2 &= ~MCG_C2_LP_MASK;
    MCG->C2 |= MCG_C2_RANGE0(3) |// Very high frequency range selected for the crystal oscillator 
               MCG_C2_EREFS0_MASK ;

    MCG->C5 &= ~MCG_C5_PRDIV0_MASK;
    MCG->C5 |= MCG_C5_PRDIV0(2 - 1); // External clock div 4
    
    MCG->C6 &= ~MCG_C6_VDIV0_MASK;
    MCG->C6 |= MCG_C6_VDIV0(24 - 24) | // Mul 24. 8 / 4 * 24 = 48MHz
               MCG_C6_CME0_MASK |
               MCG_C6_PLLS_MASK;
		
		MCG->C1 &= ~MCG_C1_CLKS_MASK;
    MCG->C1 |= ~MCG_C1_CLKS_MASK | // Output of PLL is selected for MCGOUTCLK
								MCG_C1_FRDIV(3) |
								MCG_C1_IRCLKEN_MASK;
	
    while((MCG->S & MCG_S_OSCINIT0_MASK) == 0); // wait for osc init.
    while((MCG->S & MCG_S_PLLST_MASK) != MCG_S_PLLST_MASK); // wait for PLL
    while((MCG->S & MCG_S_CLKST_MASK) != MCG_S_CLKST(3)); // wait for PLL is selected
		
		SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(2 - 1) |  // core/system clock = MCGOUTCLK / 2 = 96 / 2 = 48MHz
									 SIM_CLKDIV1_OUTDIV4(2 - 1);   // flash/bus clock = core/system / 2 = 24MHz
}

void FEI(void)
	{
		 MCG->C1 |= MCG_C1_IREFS_MASK; 							// Internal clock (32.768kHz) for FLL

    MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
    MCG->C4 |= MCG_C4_DRST_DRS(1);							// 32.768 * 732 = 24MHz
		
    MCG->C2 |= MCG_C2_RANGE0(0) ;								// Reset MCG_C2
               			
		
    MCG->C1 &= ~MCG_C1_CLKS_MASK;								// Output of FLL is selected for MCGOUTCLK
		MCG->C1 |=	 MCG_C1_CLKS(0) | 
							 	 MCG_C1_FRDIV(0)| 							// 	Divide Factor is 32
								 MCG_C1_IRCLKEN_MASK;						//	MCGIRCLK active
		
    while((MCG->S & MCG_S_IREFST_MASK) == 0); 	// wait for Internal clock is selected
    while((MCG->S & MCG_S_CLKST_MASK) != 0); 		// wait for FLL is selected
		SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(1 - 1) |  // core/system clock = MCGOUTCLK / 1 = 8 / 1 = 24MHz
    SIM_CLKDIV1_OUTDIV4(1 - 1);   // flash/bus clock = core/system / 1 = 24MHz
		SIM->SCGC5    |= (1UL << 11);        		/* Enable Clock to Port C */ 
	}
void PORTC_PORTD_IRQHandler(void) // Handler INTERRUPT SW1
{
	if(Get_Button(BT1))
		{
		FBE();
		PBE();
		PEE();
	}
			if(Get_Button(BT2))
	{	
		
		
		PBE();
		FBE();
	
	}
		PORTC->PCR[12]|= (1UL << 24);
		PORTC->PCR[3]|= (1UL << 24);			/* Clear  Flag */
}
int main()
{
	
	LED_Init(); 																		// LED init
	Button_init();																	// Init Button
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms */
	while(1)
	{
		/* Blinky led with clock by changer */
			Delay(t);								// Delay with t =250 
				LED_On(c_LED); 				// Turn On LED green
				Delay(t);
				LED_Off(c_LED);				// Turn Off LED green

	}
}
