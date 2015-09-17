/*-----------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info: Blink LEDs with dynamic change frequency
------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h" 										/*Device Header*/

#define LED_User 2												/* Led used */
#define BT1 0xfffffff7										/* Switch 1*/
#define BT2 0xffffefff										/* Switch 2*/

unsigned int  t = 500; 										/* variable Delay for LED Blinky */
unsigned char CB = 0x00; 									/* toggle */
unsigned int c_LED = 0;  									/* Choose Led :  0-Led red , 1-Ledgreen  */
/* Statup */
const char F = 0xff;  										/* Statup False   */
const char T = 0x00;											/* Statup True  */
extern void SystemCoreClockUpdate (void);

/* count 1ms timerStick */
volatile uint32_t msTicks;  
volatile uint32_t mscount;
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

/*-------------------------------------------------------------------------------*/
/* Interrupt SysTick */
/*-------------------------------------------------------------------------------*/
void SysTick_Handler(void) {
	
  msTicks++;												/* increment counter necessary in Delay() */		
	mscount++; 												/* count to 500 */ 
	if(mscount == 500) 									
	{
		CB = ~ CB; /* toggle */
		if(CB == F) /* False = 0xff */
		{
			mscount = 0; 									/* Reset variable mscount */
			LED_On(c_LED+1); 							/* ON Led Red */
		}
		else /*	 True = 0x00 */
		{
			mscount = 0;									/* Reset variable mscount */
			LED_Off(c_LED+1);							/* OFF Led Red  */
		}
		
	}	
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
/* Interrupt PORTC_PORTD */
/*-------------------------------------------------------------------------------*/
void  PORTC_PORTD_IRQHandler(void)
{
	if(Get_Button(BT2)) 							/* BUTTON PTC12 (SW2) */
	{
		t*=2;														/* Decrease frequency twice	*/					
	}
	else if(Get_Button(BT1)) 							/* BUTTON PTC3 (SW1) */
	{
		t/=2;														/* Increase frequency twice	*/
		if(t <10)
			t=250;
	}
	PORTC->PCR[3]|= (1UL << 24);			/* Clear  Flag */
	PORTC->PCR[12]|=(1UL << 24);
	
}	

int main(void){

												
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms */
	LED_Init();																		/* Init LED */
	Button_init();																/* Init Button */
	
	while(1)
	{			
		/*Blink LEDs with dynamic change frequency t */
				Delay(t);
				LED_On(c_LED); 	
				Delay(t);
				LED_Off(c_LED); 
				
	}
	
}
