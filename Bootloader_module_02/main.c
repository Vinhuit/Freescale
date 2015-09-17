#include "MKL46Z4.h" 
#include "BL_DrvLed.h"
#include "BL_DrvSw.h"
#include "BL_DrvUart.h"
#include "BL_DrvTimer.h"
#include "SysTick.h"

int led_0_state=1,led_1_state=1;
static char Srec_string[46] ;
static uint8_t Srec_line[46];
static int y = 0;
void SrecConvert();
void control();
int CheckSum(uint8_t *a);
/*----------------------------------------------------------------------------
  PIT_IRQHandler
 *----------------------------------------------------------------------------*/
void PIT_IRQHandler(void)
{
	if(PIT->CHANNEL[0].TFLG)											// check if trigger is timer0
		{
		UART0_Transmit_String("Green Status:");
		if(led_0_state) UART0_Transmit_String("ON"); else UART0_Transmit_String("OFF");
				UART0_Transmitter(10);// New line
			
		UART0_Transmit_String("Red Status:");
		if(led_1_state) UART0_Transmit_String("ON"); else UART0_Transmit_String("OFF");
				UART0_Transmitter(10);// New line
		
		UART0_Transmit_String("=====================");UART0_Transmitter(10);// New line	
		PIT->CHANNEL[0].TFLG=1;										// clear the Timer1 interrupt flag
		}
		
	if(PIT->CHANNEL[1].TFLG)											// check if trigger is timer1
		{
			TOGGLE(1);																// toggle red led
			PIT->CHANNEL[1].TFLG=1;										// clear the Timer1 interrupt flag
		}	
}

/*----------------------------------------------------------------------------
  PORTC_PORTD_IRQHandler
 *----------------------------------------------------------------------------*/
void PORTC_PORTD_IRQHandler(void)
	{
		TOGGLE(0);
		TOGGLE(1);
		led_0_state = !led_0_state;
		led_1_state = !led_1_state;
		PORTC->PCR[3]|=1UL <<24;//Xoa co ngat sw1
		PORTC->PCR[12]|=1UL <<24;//Xoa co ngat sw3
	}
/*----------------------------------------------------------------------------
  UART0_IRQHandler
 *----------------------------------------------------------------------------*/
void UART0_IRQHandler()
{
	PIT->CHANNEL[0].TCTRL |= 1UL<<0;
	Srec_string[y]= (char) UART0->D;
	y++;
	if(y == 46)
	  {
			PIT->CHANNEL[0].TCTRL &= ~1UL<<0;
		  y = 0;
		  UART0_Transmit_String("\nCommand Receive : ");
			UART0_Transmit_String(Srec_string);
			UART0_Transmit_String("\n");
			SrecConvert();
			if(CheckSum(Srec_line))
				UART0_Transmit_String("OK");
	  }
}

/*----------------------------------------------------------------------
*SrecConvert
*Convert a array char Srec_string[i] to a array of hex number Srec_line[i] 
------------------------------------------------------------------------*/
void SrecConvert()
{
	uint8_t i=0,temp=0;
	for(i=0;i<46;i++)
	{
		if( Srec_string[i] < 58 )
			temp = Srec_string[i] - 48;
		else temp = Srec_string[i] - 55;
		Srec_line[i] = temp;
	}
}
/*----------------------------------------------------------------------
*OneComplement
*return One's Complement of a value
------------------------------------------------------------------------*/
uint8_t OneComplement(uint8_t a)
{
	uint8_t temp=0x00,i=0;
	for(i=0;i<8;i++)
	{
		if(!((a>>i)&1UL)==1UL)
			temp|= 1UL<<i;
	}
	return temp;
}
/*----------------------------------------------------------------------
*CheckSum function
*Return 1 if true, 0 if false
------------------------------------------------------------------------*/
int CheckSum(uint8_t *a)
{
	int i =0,sum=0;
	for(i=2;i<44;i+=2)
	{
		sum += ((*(a+i)<<4) | (*(a+i+1)));//chia thanh tung cap roi cong voi sum
	}
	if(OneComplement((sum&0xff)) ==((*(a+44)<<4) | (*(a+44+1))))//so sanh voi cap cuoi cung
		return 1;//neu dung thi no return 1
	else return 0;
}


int main (void){
		//===========
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  
  LED_Config(); //Thiet lap LED  
	BUTTON_Config(); // Thiet lap Button
	LED_On(0); LED_Off(1);
  led_1_state = !led_1_state;
	//============
	UART0_Config();
	UART0_Transmitter(0);
	//==============
	TimerConfig();

	while(1) 
	{
	}
}

