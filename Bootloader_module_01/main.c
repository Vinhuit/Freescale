#include "MKL46Z4.h"                    // Device header
#include "stdio.h"
#include "string.h"
#include "DrvSW.h"
#include "DrvLED.h"
#include "Seg_LCD.h"
#include "DrvUART.h"

uint8_t count = 0;
uint32_t *pointer = 0;
uint32_t point_temp = 0;
char buffer[128]=" ";

uint8_t i;
uint8_t y;

volatile uint32_t msTicks;                            /* counts 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;                        /* increment counter necessary in Delay() */
}

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}


void PIT_Config(void){
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;	                     // Enable clock for PIT
	PIT->MCR = 0x00UL;			            		     // Turn on PIT
	
	PIT->CHANNEL[0].LDVAL = 0x1AAA2BF;	         // Load value Timer: 2s = (count + 1)/13,98MHz
	PIT->CHANNEL[0].TCTRL = 0x02;	       		     // Enable Interrupt Timer0
	PIT->CHANNEL[0].TFLG = 1UL<<0 ;	             // Clear Interrupt Timer0 Flag
	//PIT->CHANNEL[0].TCTRL |= 1UL<<0;	    	     // Enable Timer 0
	
	PIT->CHANNEL[1].LDVAL = 0x18FF893F;	         // Load value Timer: 30s = (count + 1)/13,98MHz
	PIT->CHANNEL[1].TCTRL = 0x02;	       		     // Enable Interrupt Timer1
	PIT->CHANNEL[1].TFLG = 1UL << 0 ;	           // Clear Interrupt Timer1 Flag
	//PIT->CHANNEL[1].TCTRL |= 1UL<<0;	    	     // Enable Timer 1
	NVIC_EnableIRQ(PIT_IRQn);
}

void PORTC_PORTD_IRQHandler()
{
  UART0_Printf("Reset count!!! ");
	count = 0;
	UART0->C2 |= UART0_C2_RE_MASK ;
	PORTC->PCR[12] |= 1UL << 24;
	PORTC->PCR[3] |= 1UL << 24;
}

void SrecConvert(char *buffer)
{

	for(i=0;i < strlen(buffer);i++)
	{
		if( buffer[i] < 58 ) //buffer[i] = 0 -> 9
			*(buffer+i) = *(buffer+i) - 48;
		else                 //buffer[i] = A-> F
		  *(buffer+i) = *(buffer+i) - 55;
	}
}

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

uint8_t CheckSum(char *a)
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

void Parse_Loaded_To_RAM(char *buffer)
{
	/* Parse address */
	for(i = 11; i>3; i--)
	{
			point_temp |= (uint32_t)((*(buffer+i))<<((11-i)*4)); //Ep tung phan tu vao con tro 32 bit
	}
	pointer = (uint32_t*)point_temp;
	/* End parse address */
	
	/* Parse data and loaded to RAM */
	for( y = 0 ; y<25 ; y = y+8)
	{
		for( i = 19; i > 11 ; i--)
		{
			i = i+y;
      point_temp |= (uint32_t)((*(buffer+i))<<((11-i)*4)); //ep data vao thanh ghi 32 bit
		}
		*pointer = point_temp;
		pointer++;
	}
	/* End parse data */
	pointer = 0;
	point_temp = 0;
	UART0_Printf("Y"); // Final
}

void Jump_To_Address(char *buffer)
{
	
}

void UART0_IRQHandler()
{
	PIT->CHANNEL[0].TCTRL |= 1UL<<0; //Enable Timer 0
	buffer[count] = (char)UART0->D;
	count++;
	/* add code know connect or disconect */
	
	if(buffer[count-1] == 0x0A) // nhan duoc ky tu \n
	{
		
		SrecConvert(buffer);
		if (CheckSum(buffer))          //check thanh cong
		  Parse_Loaded_To_RAM(buffer);
		else                           //check khong thanh cong
			UART0_Printf("N");
		count = 0;
	}
	if (buffer[count-1] == 0x53)
	{
		PIT->CHANNEL[0].TCTRL &= ~1UL<<0;
		Jump_To_Address(buffer);
		count = 0;
	}
}

void PIT_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(PIT_IRQn);
	if(PIT->CHANNEL[0].TFLG != 0 )	            		// Check Interrupt Timer0 Flag 
	{	
		UART0_Printf("N");
		PIT->CHANNEL[0].TCTRL &= ~1UL<<0;
	  count = 0;
		PIT->CHANNEL[0].TFLG = 1UL<<0 ;	              // Clear Interrupt Timer0 Flag
	}
	if (PIT->CHANNEL[1].TFLG != 0)									// Check Interrupt Timer0 Flag
	{
		PIT->CHANNEL[1].TFLG = 1UL << 0 ;		 
	}
}

int main()
{
	SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
  SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms    */
  DrvSW_Interrupt_Config();
	DrvLED_Config();
	DrvUART0_Config();
	SegLCD_Init();
	PIT_Config();
	Delay(1);
	DrvLED_On(1);
	while(1)
	{
		//SegLCD_DisplayDecimal(count);
	}
}
