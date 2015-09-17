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
#define max 0
int t,n,z=0;
char c;	
int g=0;
char *hexstring = "abcd";


int hex[5412];
char *Data;
char buffer[5412];

int count =0;
char *get_string(void);
unsigned char TG =0xFF;
#define MAX_LEN 0x100       // Some security, use your own limits
//int string_length(char * str){
//    int len = 0, i;
//    for(i = 0; (str[i] != 0) && (i < MAX_LEN);i++)
//							len++;
//	 
//    return len;
//}

/*-----------------------------------------------------*/
/* Delay */
/*-----------------------------------------------------*/
volatile uint32_t msTicks; /* count 1ms timerStick */
void SysTick_Handler(void) {
  msTicks++;  								  /* increment counter necessary in Delay() */
}
/*Delay number of tick stick (1ms)*/
static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}
void charToHex(char c[46], int hex[46])
{int i;
	for(i=0;i<48;i++)
	{
    if(c[i]<='9' && c[i]>='0')
			hex[i] = c[i]-18;
		if(c[i]>='A' && c[i]<='F')
			hex[i] = c[i]- 24;
		
	}
}
int str_cut(char *str, int begin, int len)
{
    int l = strlen(str);

    if (len < 0) len = l - begin;
    if (begin + len > l) len = l - begin;
    memmove(str + begin, str + begin + len, l - len + 1);

    return len;
}
int Check_Sum(char c[5412],int temp,int ich,int hex[5412])
{

	int i,j=0,sums=0,r, sum[22], s[5412];
		
	if(c[temp] !=0&&c[temp+1]!=0)
	{
	for(i=temp+2;i<temp+47;i++)
	{
	if(c[i]<='9' )
		s[i]=c[i]-48;
	if(c[i]>='A' )
		s[i]=c[i]-55;
	}
	for(i=temp+2;i<temp+45;i+=2)
{
	sum[j]=((s[i]*16)|s[i+1]);
	j++;

}
if(ich == 3)
{
for(i=0;i<22;i++)
{
	hex[z]=sum[i];
	z++;

}
}
if(ich == 1)
{
for(i=0;i<18;i++)
{
	hex[z]=sum[i];
	z++;

}
}
if(ich == 7)
{
for(i=0;i<5;i++)
{
	hex[z]=sum[i];
	z++;

}
}
if(ich == 3)
{
for(r=0;r<21;r++)
				sums+=sum[r];
}
if(ich == 1)
{
for(r=0;r<17;r++)
				sums+=sum[r];
}
if(ich == 7)
{
for(r=0;r<5;r++)
				sums+=sum[r];
}
}else
	return 0;

//if(ich == 3)
//{
//if((~sums&0x00FF)==sum[21])
//printf("T");

//else 
//	printf("F");
//}else if(ich == 1)
//{
//if((~sums&0x00FF)==sum[17])
//printf("T");
//else 
//	printf("F");

//}else if(ich == 7)
//{
//if((~sums&0x00FF)==sum[5])
//printf("T");
//else 
//	printf("F");

//}
return sums;

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
//void Load_To_Ram(char Data[6000])
//{
//	int e,s,count;
//		unsigned char i;
//		unsigned char *iData;
//		unsigned char iAddress[200];
//		for(e=0;e<6000;e++) //5412
//	{
//			if(Data[e] == 'S')
//				for(s=tm0;s<8;s++)
//			{
//				iAddress[s] = Data[e+4];
//				e++;
//				
//			}
//	}
//	
//	


//}
void PORTC_PORTD_IRQHandler(void) // Handler INTERRUPT SW1
{
	int e;
	

	if(Get_Button(BT1))
	{
			TG=~TG; //Bien dao
	if(TG == 0x00)
	{
			UART0->C2 |= UART0_C2_RE_MASK;
		LED_Off(1);
	}
	else
	{
			UART0->C2 &= ~UART0_C2_RE_MASK;
		
	}
}
	if(Get_Button(BT2))
	{
		LED_Off(0);
//			for(e=0;e<48;e++)
//			{
//				
//				printf("%c",buffer[e]);
//			}
		for(e=0;e<5412;e++) //5412
				{
					if(e%22 == 0)
					printf("\n");
					printf("%02x",hex[e]);
				}
	
	}
		PORTC->PCR[12]|= (1UL << 24);
		PORTC->PCR[3]|= (1UL << 24);			/* Clear  Flag */
	
}


void PIT_IRQHandler(void)
{ int a,e=0,temp=0,n=0;

	/* every 30s toggle LED */
	/* Interrupt PIT1 */
	if(PIT->CHANNEL[0].TFLG == 1)
	{
			if(c == 'W')
			{
				
			UART0->C2 &= ~UART0_C2_RE_MASK;
			}
			for(e=0;e<5412;e++) //5412
				{
							if(buffer[e] == 'S'&& buffer[e+1]=='3'&& buffer[e+3] == '5')
							{ 
								a=Check_Sum(buffer,e,3,hex);
							//	printf("%d\n",a);
								
							}
							if(buffer[e] == 'S'&& buffer[e+1]=='3' && buffer[e+3] == '1')
							{ 
								a=Check_Sum(buffer,e,1,hex);
							//	printf("%d\n",a);
								
							}
							if(buffer[e] == 'S'&& buffer[e+1]=='7')
							{ 
								a=Check_Sum(buffer,e,7,hex);
							//printf("%d\n",a);
								
							}
				}
				
				
		
		
	
		count ++;
		
		PIT->CHANNEL[0].TFLG =0x01; // clear flag
	}
	if(PIT->CHANNEL[1].TFLG == 1)
	{
		
	
		PIT->CHANNEL[1].TFLG =0x01; // clear flag
	}
	
			
		
				
			
}
		

void UART0_IRQHandler(void)
{int e,a;
if(g>5412) //size 45
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

	uint32_t e;
 /* Config LED,BUTTON,LCD,PIT */
SystemCoreClockUpdate();                      /* Get Core Clock Frequency */
	SysTick_Config(SystemCoreClock/1000);         /* Generate interrupt each 1 ms */
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
		
	
	}
}