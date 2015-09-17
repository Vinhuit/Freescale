/*-----------------------------------------------------------------------------------------------------------
[GST2015]

Name : Huynh Ngoc Vinh
MS: 12520784
Info:Keil project for the FRD KL46Z256 which uses printf() function to send a “Hello world” 
to PC using UART module of the FRD KL46Z256.
-	Baud rate: 115200, Data bit: 8; Parity: None; Stop bits: 1; Flow Control: None.

-----------------------------------------------------------------------------------------------------------*/
#include "MKL46Z4.h"                    // Device header
#include "_uart.h"
#include "stdio.h"
#include "PIT_TM.h"
void MCG_FEI(void); // Using FEI 48MHZ

int main(void)
{

  
    MCG_FEI();
    Uart0_init(115200); // selection baud with FEQ =48MHZ
		/*	-	Baud rate: 115200, Data bit: 8; Parity: None; Stop bits: 1; Flow Control: None. */
		Pit_init();				//enable pit interrupt blinky led
		
    while(1)
    {
			
			printf("\n\r Hello World"); // Transmit "Hello World" by UART0 
			
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

