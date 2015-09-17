#include "MKL46Z4.h" 
volatile uint32_t msTicks;                            /* bien dem 1ms timeTicks */

/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Name: Delay
Delay mot so cho truoc 
 *------------------------------------------------------------------------------*/
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;// gia tri hien tai

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

