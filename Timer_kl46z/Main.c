/*-----------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info: Implement the code to control LEDs by buttons. Press button to turn on LED; Release button to turn off LED
-------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h"
//#include "isr.h"
//#include "LCDs.h"
#include "Seg_LCD.h"
#include "MemMapPtr_KL46Z4.h"

#define LED_User 1 												/* Led red */
#define SW1 0xfffffff7										/* Switch 1*/
#define SW2 0xffffefff

unsigned int t =250;
	unsigned int c_LED = 0;
const unsigned char F = 0xff;
const unsigned char T = 0x00;
extern void SystemCoreClockUpdate (void);
volatile uint32_t msTicks;
uint16_t count_mseg=125;

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
		PORTC->PCR[12] = (1UL << 8)|(1UL << 1)|(1UL << 0);	
}

static int Get_Button(uint32_t key){
uint32_t GT;
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
void Timer_Config(void){
	TPM0->SC = (1UL << 6)|(1UL << 2)|(1UL << 1)|(1UL << 3);
	TPM0->CNT = 0;
	
		//TPM0->MOD = 131;

	SIM->SCGC6 |=SIM_SCGC6_TPM0_MASK; 
	//TPM0->STATUS = 0x100;
	TPM0->MOD = 0x12C0;
	
		NVIC_EnableIRQ(17);
	TPM0_SC |= TPM_SC_TOF_MASK;
}
void tpm0_isr(void)
{
	LED_On(1);
	LED_On(0);
	TPM0_STATUS |=TPM_SC_TOF_MASK;
}
void TPM0_IRQHandler(void)
{
		
	LED_On(1);
	LED_On(0);

	
	TPM0_SC |= TPM_SC_TOF_MASK;
}

void Pit_init(void)
{
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // enable PIT module
    
    /* Enable PIT Interrupt in NVIC*/   
   NVIC_EnableIRQ(PIT_IRQn);
       
    PIT_MCR = 0x00;  // MDIS = 0  enables timer
    PIT_TCTRL1 = 0x00; // disable PIT0
    PIT_LDVAL1 = 0x02625A00; // 
    PIT_TCTRL1 = PIT_TCTRL_TIE_MASK; // enable PIT0 and interrupt
    PIT_TFLG1 = 0x01; // clear flag
    PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK;
   
}

/**   Pit1_isrv
 * \brief    Periodic interrupt Timer 1.  Interrupt service
 * \brief    PIT1 is used for tone/buzzer time control
 * \author   b01252
 * \param    none
 * \return   none
 */  

void PIT_IRQHandler(void)
{  
    
  c_LED+=1;
	
	if(c_LED == 2)
		c_LED = 0;
	if(c_LED == 0)
	LED_On(0);
	if(c_LED == 1)
		LED_Off(0);
	PIT_TFLG1 = 0x01; // clear flag
}

int main(void){

	
	unsigned char CB = 0x00;

	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  BUTTON_Config();
	LED_Config(); 
	//Timer_Config();
	Pit_init();
	
	SegLCD_Init();

	
 	while(1)
	{
			SegLCD_Col_On();
			Delay(300);
			SegLCD_Col_Off();
			Delay(300);
		
	}	
	}
	
