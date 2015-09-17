/*-----------------------------------------------------------------------------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info: Digital Clock LCD 
. Using SW1 to choose : 1-hour ,2-min, 3-Normal Clcck
. Using SW3 to increase value hour or min in LCD.

-----------------------------------------------------------------------------------------------------------*/
#include  <stdio.h>
#include  "MKL46Z4.h"
#include "Seg_LCD.h"
#include "Button.h"
#include "PIT_TM.h"
#include "LED.h"
#include "time.h"

unsigned char TG =0xFF;
int hour = 00;
int min = 00;
int second =0;
int t,n;

extern void SystemCoreClockUpdate (void);
volatile uint32_t msTicks;


/*Delay*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}
static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}


void PORTC_PORTD_IRQHandler(void) // Handler INTERRUPT SW1
{
	
	if(Get_Button(BT1)) // Get SW1 to choose mode :1-hour, 2-min,3-normal clock
		t+=1;
	else if(Get_Button(BT2))	// increase hour or min
		n ++;
	if(t>3)	//get mode 
		t=1;
		PORTC->PCR[12]|= (1UL << 24);
		PORTC->PCR[3]|= (1UL << 24);			/* Clear  Flag */
}
void PIT_IRQHandler(void)
{ 
	/*  PIT timer interrupt every 1s */
	TG=~TG; // toggle LED 1s
	second +=1; // increase second 
	if(second == 60) // increase min when second =60
	{
		min +=1;
		second =0; //reset second
	}
	else if(min == 60) // increase hour when min =60
	{
		min =0; //reset min
		hour+=1;
	}
	if(hour == 24) // Reset hour when hour =24
		hour = 0;
		LED_Toggle(1);
	
  PIT->CHANNEL[0].TFLG = 0x01; // clear flag
			
	
	
	}
int main()
{
	
 /* Config LED,BUTTON,LCD,PIT */
	LED_Init(); 
	Button_init();
	SegLCD_Init();
	Pit_init();
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
	
	while(1)
	{
		/* Mode Choose */
	switch(t)
	{
		case 1: n = hour;
					
						while(Get_Button(BT1)== 0) // Set hour Show DP1 
						{
						if(n>23)
						n=0;
						SegLCD_Set(n/10, 1);  // 
						SegLCD_Set(n % 10, 2);
						SegLCD_DP1_On();
						SegLCD_DP3_Off();
						Delay(100);
						
						hour = n;
						}
						break;
		case 2:	
						n = min;
						while(Get_Button(BT1)== 0) // Get min Show DP3 
						{
						if(n>59)
							n=0;
						SegLCD_Set(n/10, 3);
						SegLCD_Set(n % 10, 4);
						SegLCD_DP1_Off();
						SegLCD_DP3_On();
						Delay(50);
						min =n;
						}
						break;
		case 3: t=3 ;  // Exit Mode
						break;
	}
	SegLCD_DisplayTime(hour,min); // Show digital clock to LCD
	
	//Blinky COL LCD 
	if(TG == 0x00)
	{
	SegLCD_Col_On();
	}
	else
	SegLCD_Col_Off();
		
	}
}
