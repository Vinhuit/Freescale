/*-----------------------------------------------------------------------------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info:Final Test


-----------------------------------------------------------------------------------------------------------*/
#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  "MKL46Z4.h"
#include 	"LED.h"
#include  "Button.h"
#include "_uart.h"
#include "PIT_TM.h"
unsigned int t,n,z=0,s2=0,s3=0,x=0;
int check;
char c;	
int g=0;

unsigned char Hex[3000];
char buffer[47];

unsigned char TG =0xFF;
void Check_Sum(char c[47],unsigned char hex[3000])
{
	int i,j=0,sums=0,sum[47], s[47];
	if(c[0] =='S')
	{
	for(i=2;i<47;i++)
	{
	if(c[i]<='9')
		s[i]=c[i]-48;
	if(c[i]>='A')
		s[i]=c[i]-55;
	}
	for(i=2;i<45;i+=2)
{
	sum[j]=((s[i]*16)|s[i+1]);
	j++;
}

	for(i=0;i<sum[0]+1;i++)
{
	hex[z]=sum[i];
	z++;
}

for(i=0;i<sum[0];i++)
		sums+=sum[i];
}

if((~sums&0x00FF)==sum[sum[0]])
//check =1;
printf("T");
else 
//	check =0;
printf("F");
}
void StartApplication( void (*StartAddress)() )
{
     (*StartAddress)();
		
}

uint32_t getAdrr(unsigned char *ptrData){
 return ((ptrData[1] << 24) | (ptrData[2] <<16)| (ptrData[3] <<8) | (ptrData[4])) ;
 }
void Load_To_Ram(unsigned char *Data)
{
	int e;
		uint32_t temp;
		unsigned char i;
		unsigned char *ptr;
		unsigned int leghth;
		unsigned int  	m_StartAddress = 0x1FFFE0C1; 
	

		for(e=0;e<2579;e++) //5412
		{
			if((Data[e] != 0x05)&& Data[e+1] == 0x1f)
			{
				leghth =Data[e];
				temp=getAdrr(&Data[e]);
				ptr= (unsigned char *)temp;
				for(i=0;i<(leghth-5);i++)
				{
				ptr[i]= Data[e+5+i];
				
				}
				e=e+leghth;
			}
			if(Data[e] == 0x05 && Data[e+1] == 0x1f)
			{
				StartApplication( (void (*)())m_StartAddress );
				
			}
		}

}


void PORTC_PORTD_IRQHandler(void) // Handler INTERRUPT SW1
{
	
	TG=~TG; //Bien dao
	if(TG == 0x00)
	{
		Load_To_Ram(Hex);
	}

		PORTC->PCR[12]|= (1UL << 24);
		PORTC->PCR[3]|= (1UL << 24);			/* Clear  Flag */
}


void PIT_IRQHandler(void)
{ 

	/* every 30s toggle LED */
	/* Interrupt PIT1 */
	if(PIT->CHANNEL[0].TFLG == 1)
	{
			if(c == 'W')
			{
				LED_On(0);
				printf("E");
			}

		
		PIT->CHANNEL[0].TFLG =0x01; // clear flag
	}
		}

void UART0_IRQHandler(void)
{
int e;
	
	if (UART0->S1&UART_S1_RDRF_MASK)
	{
		
		c = UART0->D;
		buffer[g]=c;
		g++;
		
	}
	
if(g==47) //size 45
{
			
for(e=0;e<47;e++)
	{
		if(buffer[e] == 'S')
			{ 
		
				Check_Sum(buffer,Hex);
								
		  }								
	}

	g=0;
}
}

void MCG_FEI(void)
{
    MCG->C1 |= MCG_C1_IREFS_MASK; // Internal clock (32.768kHz) for FLL

    MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
    MCG->C4 |= MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(1);// 32.768 * 1464 = 47972.352kHz ~ 48MHz
    
    MCG->C1 &= ~MCG_C1_CLKS_MASK; // Output of FLL is selected for MCGOUTCLK

    while((MCG->S & MCG_S_IREFST_MASK) == 0); // wait for Internal clock is selected
    while((MCG->S & MCG_S_CLKST_MASK) != 0); // wait for FLL is selected
}

int main()
{
 /* Config LED,BUTTON,LCD,PIT */
		
	LED_Init(); 
	Button_init();
	Pit_init();
	
  MCG_FEI();
 Uart0_init(115200); // selection baud with FEQ =48MHZ
				LED_On(1); //LED DO;
	/*	-	Baud rate: 115200, Data bit: 8; Parity: None; Stop bits: 1; Flow Control: None. */
	// Load_To_Ram(Hex);

	while(1)
	{

			
		
	
	}
}
