/*------------------------------------------
UIT - CE07
Tong Tri Kien - 12520210

Using for control LED in FRDM-KL46Z
-------------------------------------------*/

#ifndef __DRV_LED_H_
#define __DRV_LED_H_

#include "MKL46Z4.h"

//Function Declarations
void DrvLED_Config(void);
void DrvLED_On (uint32_t led);
void DrvLED_Off (uint32_t led);
void DrvLED_Toggle(uint32_t led);

#endif
