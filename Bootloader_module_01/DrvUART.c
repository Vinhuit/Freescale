#include "MKL46Z4.h"
#include "DrvUART.h"
#include "String.h"

void DrvUART0_Config(void)
{
	uint32_t divisor;
	
	// enable clock
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_SHIFT ;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(0x01);
	PORTA->PCR[1] = PORT_PCR_MUX(0x2);
	PORTA->PCR[2] = PORT_PCR_MUX(0x2);
	SIM->SCGC4 |= 1UL<<10;
	
	//disabled tx and rx before configuration
	UART0->C2 &= ~UART0_C2_TE_MASK ;
	UART0->C2 &= ~UART0_C2_RE_MASK ;
	
	// Data bit: 8; Parity: None; Stop bits: 1; Flow Control: None
	UART0->C1 = 00U;
	UART0->C3 = 00U;
	UART0->C4  = UART0_C4_OSR(15);
	UART0->S2 = 00U;
	UART0->S1 = 00U;
	
  // Set baud rate 115200
	divisor = 23;
	UART0->BDH = UART0_BDH_SBR(divisor>>8); 
	UART0->BDL = (uint8_t)(divisor);
	
	// Tx & Rx are enable
	UART0->C2 |= UART0_C2_TE_MASK ;
	UART0->C2 |= UART0_C2_RE_MASK ;
	
	UART0->C2 |= UART0_C2_RIE_MASK ;
	
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
}

void UART0_Transmit_Poll(uint8_t data)
{
		/* Wait until space is available in the FIFO */
    while(!(UART0->S1 & UART0_S1_TDRE_MASK));

    /* Send the character */
    UART0->D = data;
}

void UART0_Printf(char* data)
{
	int i = 0;
	for (i = 0; i<strlen(data);i++)
		{
			UART0_Transmit_Poll((uint8_t)(*(data+i)));
		}
}
