/*------------------------------------------
UIT - CE07
Tong Tri Kien - 12520210

Using for control SW1 and SW3 in FRDM-KL46Z
-------------------------------------------*/

#ifndef __DRV_SW_H_
#define __DRV_SW_H_

#include "MKL46Z4.h"

//Function Declarations
void DrvSW_Config(void);
void DrvSW_Interrupt_Config(void);
uint32_t DrvSW_Get(void);

#endif
