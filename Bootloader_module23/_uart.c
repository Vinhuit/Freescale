
#include "MKL46Z4.h"                    // Device header
#include "_uart.h"
#include "stdio.h"

void Uart0_init(uint32_t baud)  // confirm uart0
{
	uint32_t clk =48000000; //CLock source
	uint16_t temp = 0; //Save SBR
	uint8_t BDL; 				//save  UART0_BDL
	uint8_t BDH;				//save  UART0_BDH
	 SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;  // Set clock PORTA
   SIM->SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // MCGFLLCLK is selected for UART0 clock source 
   SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);		
	
    /* Enable the UART_TXD function on PTA1 */
    PORTA->PCR[1] = PORT_PCR_MUX(0x2);

    /* Enable the UART_TXD function on PTA2 */
    PORTA->PCR[2] = PORT_PCR_MUX(0x2);
	
		SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // enable UART0 Clock 
		UART0->C4 = 0x0F;  					// UART0_C4[OSR]= 0x0F  => ratio 16x
		temp = clk/(baud*16); 			// SBR 
		BDH = temp>>8;							//5 bit High SBR
		BDL = temp & 0x00ff;				//8 bit LOW 	SBR
		UART0->BDH = BDH;		
		UART0->BDL = BDL;
	

		UART0->C1 = 0x00;						//Default control uart

		UART0->C5|= UART0_C5_BOTHEDGE_MASK; //Both Edge Sampling
		/* enable UART Transmitter and Receiver */
		UART0->C2 |= UART0_C2_RIE_MASK;
		UART0->C2 |= (UART0_C2_TE_MASK  ); 
		NVIC_EnableIRQ(UART0_IRQn);
	
}
void uart0_putchar (char ch) //Put char 
{
    /* Wait until space is available in the FIFO */
    while(!(UART0->S1 & UART0_S1_TDRE_MASK));

    /* Send the character */
    UART0->D = (uint8_t)ch;
	
}

char uart0_getchar(void)
{
      /* Wait until character has been received */
      while (!(UART0->S1 & UART0_S1_RDRF_MASK));
    	if (!(UART0->S1&UART_S1_TDRE_MASK) && !(UART0->S1&UART_S1_TC_MASK))
      /* Return the 8-bit data from the receiver */
      return UART0->D;

}

int uart0_getchar_present (void)
{
    return (UART0->S1 & UART0_S1_RDRF_MASK);
}
int fputc(int c, FILE *f)
{
  uart0_putchar(c);
	return (c & 0xFF);
}
int fgetc(FILE *f)
{
  
  return (uart0_getchar());
}
