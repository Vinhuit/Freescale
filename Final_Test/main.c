/*-----------------------------------------------------------------------------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info:Final Test


-----------------------------------------------------------------------------------------------------------*/
#include  <stdio.h>
#include <string.h>
#include  "MKL46Z4.h"
#include 	"LED.h"
#include "Button.h"
#include "_uart.h"
#include "PIT_TM.h"
int t,n;
char c;	
int g=0;
int e ,d,i;
char buffer[8];

int count =0;
char command[4][8] = {"SLED1ONE","SLED1OFE","SLED2ONE","SLED2OFE"};

char *get_string(void);
unsigned char TG =0xFF;
#define MAX_LEN 0x100       // Some security, use your own limits
int string_length(char * str){
    int len = 0, i;
    for(i = 0; (str[i] != 0) && (i < MAX_LEN);i++)
							len++;
	 
    return len;
}
int string_compare(char * str1, char * str2){
    int l1, l2, i;          // Length of both strings, loop index
   
   
    for(i = 0; i < 8; i++)
	{
   if(str1[i] != str2[i])  
	 { 
		return 0;
		break;
	 }
    }
	if(i==8)
			return 1;
    
   
}
void PORTC_PORTD_IRQHandler(void) // Handler INTERRUPT SW1
{
	
	TG=~TG; //Bien dao
	if(TG == 0x00)
	{
		LED_On(1); // turn on led red
		LED_Off(0); // turn off led green
		t =1;
	}
	else
	{
		LED_On(0); // turn on led green
		LED_Off(1); // turn off led red
		t = 0;
		
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
			
		
		for(e=0;e<8;e++)
		printf("%c",buffer[e]);
		printf("%d",i);
		count ++;
		
		PIT->CHANNEL[0].TFLG =0x01; // clear flag
	}
	if(PIT->CHANNEL[1].TFLG == 1)
	{
		
	if(t==1)
	{
		
		printf("\nGreen Status: OFF");
		printf( "\nRED Status: ON");
	}
		else
		{
		printf("\nGreen Status: ON");
		printf( "\nRED Status: OFF");
		}
		PIT->CHANNEL[1].TFLG =0x01; // clear flag
	}
	
			
		
				
			
			
		}

void UART0_IRQHandler(void)
{
	int i[4];
if(g>7)
{

g=0;
}
if (UART0->S1&UART_S1_RDRF_MASK)
{
c = UART0->D;
if (!(UART0->S1&UART_S1_TDRE_MASK) && !(UART0->S1&UART_S1_TC_MASK))
{
UART0->D = c;
}
}
	buffer[g]=c;
g++;

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
int i[4],d,e;
 /* Config LED,BUTTON,LCD,PIT */
	LED_Init(); 
	Button_init();
	Pit_init();
	
  MCG_FEI();
 Uart0_init(115200); // selection baud with FEQ =48MHZ
	/*	-	Baud rate: 115200, Data bit: 8; Parity: None; Stop bits: 1; Flow Control: None. */
	 
	LED_On(1); //LED DO;
	LED_On(0); //LED Xanh
	while(1)
	{
		
		i[0] =string_compare(buffer,command[0]);
		i[1] =string_compare(buffer,command[1]);
		i[2] =string_compare(buffer,command[2]);
		i[3] =string_compare(buffer,command[3]);
		if(i[0] == 1)
			LED_On(1);
	
	if(i[1] == 1)
			LED_Off(1);
	
	if(i[2] == 1)
			LED_On(0);

	if(i[3] == 1)
			LED_Off(0);

	}
}