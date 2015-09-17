#include <stdio.h>
#include "MKL46Z4.h"
#include "string.h"
#include "BL_DrvUart.h"


void UART0_Transmitter(uint8_t data)
{	
    while(!(UART0->S1 & UART0_S1_TDRE_MASK));
    UART0->D = data;
}

void UART0_Transmit_String(char* a)
{
	int i,tmp;
	for(i=0; i<strlen(a);i++)
	{
		tmp = (uint8_t)(*(a+i));
		UART0_Transmitter(tmp);
	}
}
uint8_t UART0_Receiver()
{	
	while(!(UART0->S1 & UART0_S1_RDRF_MASK));
	return UART0->D;
}

void UART0_Config()
{
		uint32_t baudrate;
		/*SET CLOCK FOR UART*/
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; 			// turn on port A
		SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; 	// MCGFLLCLK is selected for UART0 clock source 
		SIM->SOPT2 |= SIM_SOPT2_UART0SRC(0x1);		// Selects the clock source for the UART0 transmit and receive clock is MCGFLLCLK clock
		PORTA->PCR[1] = PORT_PCR_MUX(0X2);				// GPIO
		PORTA->PCR[2] = PORT_PCR_MUX(0X2);				// GPIO
		SIM->SCGC4 |= 1UL<<10;										// Enable Clock UART0
	
		/* Disable transmitter. */
		UART0->C2 &= ~UART_C2_TE_MASK;
		UART0->C2 &= ~UART_C2_RE_MASK;
	
		/* UART0_C1: LOOPS=0,DOZEEN=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
		UART0->C1 = 0x00U;                    /*  Set the C1 register */
		/* UART0_C3: R8T9=0,R9T8=0,TXDIR=0,TXINV=1,ORIE=0,NEIE=0,FEIE=0,PEIE=0 */
		UART0->C3 = 0x00U;                    /*  Set the C3 register */
		/* UART0_C4: MAEN1=0,MAEN2=0,M10=0,OSR=0 */
		UART0->C4 = UART0_C4_OSR(15);       /*  Set the C4 register */
		/* UART0_S2: LBKDIF=0,RXEDGIF=0,MSBF=0,RXINV=1,RWUID=0,BRK13=0,LBKDE=0,RAF=0 */
		UART0->S2 = 0x00U;                    /*  Set the S2 register */
		UART0->S1 = 00U;
		/*baudrate_value = CoreClock/(OSR*Baudrate) = 41.94MHz/(16*115200) = 23  */
		baudrate = 23; 
		UART0->BDH = UART0_BDH_SBR(baudrate >> 8); 
		UART0->BDL = (uint8_t) baudrate;
		/* Enable transmitter. */
		UART0->C2 |= UART0_C2_TE_MASK;
		UART0->C2 |= UART_C2_RE_MASK;
		
		UART0->C2 |= UART0_C2_RIE_MASK ;
		NVIC_SetPriority(UART0_IRQn,1);
	  NVIC_EnableIRQ(UART0_IRQn);
	  NVIC_ClearPendingIRQ(UART0_IRQn);
}



