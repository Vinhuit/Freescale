/*HEADER**********************************************************************
*
* Copyright 2011 Freescale Semiconductor, Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains timer functions for use with a PIT.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "timer_pit.h"


#define PIT_PCSR_PRESCALE_SHIFT 8
#define PIT_PCSR_DOZE           (1<<6)
#define PIT_PCSR_HALTED         (1<<5)
#define PIT_PCSR_OVW            (1<<4)
#define PIT_PCSR_PIE            (1<<3)
#define PIT_PCSR_PIF            (1<<2)
#define PIT_PCSR_RLD            (1<<1)
#define PIT_PCSR_EN             (1<<0)


typedef struct pit_struct
{
    uint16_t PCSR;   /* PIT control and status register */
    uint16_t PMR;    /* PIT modulus register */
    uint16_t PCNTR;  /* PIT counter register */
} PIT_STRUCT, * PIT_STRUCT_PTR;


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _pit_init_freq
* Returned Value  : hw ticks per tick
* Comments        :
*   this function will set up a timer to interrupt
*
*END*---------------------------------------------------------------------*/

uint32_t _pit_init_freq
    (
        /* [IN] the timer to initialize */
        uint8_t    timer,

        /* [IN] ticks per second */
        uint32_t   tickfreq,

        /* [IN] input clock speed in Hz */
        uint32_t   clk,

        /* [IN] unmask the timer after initializing */
        bool   unmask_timer
    )
{
    uint32_t rate, prescale = 0;

    volatile PIT_STRUCT       *pit_ptr;

    pit_ptr = _bsp_get_pit_base_address(timer);
    if (pit_ptr == NULL) return 0;

    _bsp_int_init(_bsp_timers[timer].INTERRUPT, _bsp_timers[timer].LEVEL, _bsp_timers[timer].PRIORITY, FALSE);

    rate = (clk / tickfreq);
    while (rate > (0xFFFF+1)) {
        prescale += 1;  /* divides clock in half */
        clk >>= 1;
        rate = (clk / tickfreq);
    }

    pit_ptr->PCSR = (prescale << PIT_PCSR_PRESCALE_SHIFT)
        | PIT_PCSR_HALTED             /* stop timer when halted */
        | PIT_PCSR_PIF                /* Clear interrupt if present */
        | PIT_PCSR_OVW                /* Overwrite count with PMR when written */
        | PIT_PCSR_OVW                /* Enables writing to PMR */
        | PIT_PCSR_PIE                /* PIT interrupt Enable */
        | PIT_PCSR_RLD;               /* Enabled Reload bit */

    /* Set counter reload value and counter value */
    pit_ptr->PMR = (uint16_t)(rate-1);

    if (unmask_timer) {
        _bsp_int_enable(_bsp_timers[timer].INTERRUPT);
    }

    /* Enable the timer */
    pit_ptr->PCSR |= PIT_PCSR_EN;

    /* return number of hw-ticks per MQX tick */
    return rate;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _pit_get_hwticks
* Returned Value  : Number of ticks
* Comments        :
*   This function will return the number of ticks elapsed since the last
* timer tick.
*
*END*---------------------------------------------------------------------*/

uint32_t _pit_get_hwticks
    (
        /* [IN] the timer to use */
        void   *parameter
    )
{
    uint16_t pmr;
    uint32_t ticks;
    
    volatile PIT_STRUCT       *pit_ptr;

    pit_ptr = _bsp_get_pit_base_address((uint8_t)parameter);
    if (pit_ptr == NULL) return 0;

    pmr   = pit_ptr->PMR;
    ticks = pmr - pit_ptr->PCNTR;

    if ((pit_ptr->PCSR & PIT_PCSR_PIF) != 0) {
        /* Another full TICK period has expired since we handled the last timer interrupt.
        We need to read the counter again, since the wrap may have
        occurred between the previous read and the checking of the overflow bit. */
        ticks = pmr + (pmr-pit_ptr->PCNTR) + 1;
    }
   
    return ticks;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _pit_clear_int
* Returned Value  :
* Comments        :
*   This function will clear interrupt flag of given timer
*
*END*---------------------------------------------------------------------*/

void _pit_clear_int
    (
        /* [IN] the timer to use */
        uint8_t    timer
    )
{
    volatile uint8_t *pcsr_low;
    volatile PIT_STRUCT       *pit_ptr;

    pit_ptr = _bsp_get_pit_base_address(timer);
    if (pit_ptr == NULL) return;

#if (PSP_ENDIAN == MQX_BIG_ENDIAN)
    pcsr_low = ((uint8_t *)(&pit_ptr->PCSR)) + 1;
#else
    pcsr_low = ((uint8_t *)(&pit_ptr->PCSR));
#endif

    *pcsr_low |= PIT_PCSR_PIF;
}
