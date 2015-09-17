
/*------------------------------------------
UIT - CE07
Tong Tri Kien - 12520210

Using for control SW1 and SW3 in FRDM-KL46Z
-------------------------------------------*/

#ifndef __DRV_UART_H_
#define __DRV_UART_H_

#include "MKL46Z4.h"

//Function Declarations
void DrvUART0_Config(void);
void UART0_Transmit_Poll(uint8_t data);
void UART0_Printf(char* data);

#endif
