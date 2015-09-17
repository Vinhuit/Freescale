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
*   This file contains timer functions for use with a MTIM.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "timer_mtim16.h"


#define MTIM16_SC_TSTP_MASK  (1<<4)
#define MTIM16_SC_TRST_MASK  (1<<5)
#define MTIM16_SC_TOIE_MASK  (1<<6)
#define MTIM16_SC_TOF_MASK   (1<<7)

static void _mtim16_kernel_isr(void *p);

typedef struct mtim16_struct
{
    uint8_t MTIMSC;   /* Status/control Register */
    uint8_t MTIMCLK;  /* Clock Configuration Register */
    uint16_t MTIMCNT;  /* Counter Register */
    uint16_t MTIMMOD;  /* Modulo Register */      
} MTIM16_STRUCT, * MTIM16_STRUCT_PTR;


_mqx_int _mtim16_timer_install
(
  /* [IN] the timer to initialize */
  uint8_t    timer,

  /* [IN] ticks per second */
  uint32_t   tickfreq,

  /* [IN] input clock speed in Hz */
  uint32_t   clk,

  /* [IN] interrupt priority */
  uint32_t priority,

  INT_ISR_FPTR isr_ptr,
    
  /* [IN] unmask the timer after installation */
  bool   unmask_timer
)
{
    uint32_t result;
    _mqx_uint vector = _bsp_get_mtim16_vector(timer);

    if (vector == 0)
    {
        return MQX_INVALID_DEVICE;
    }

    _bsp_int_disable(vector);

    /* Set up tick timer */
    _mtim16_timer_init(timer, tickfreq, clk, FALSE);

    /* Install the timer interrupt handler */
    if (_int_install_isr(vector, isr_ptr, NULL) == NULL)
	{
		return MQX_TIMER_ISR_INSTALL_FAIL;
	}

    _bsp_int_init(vector, priority, 0, TRUE);
    
    _bsp_int_enable(vector);

    if (unmask_timer) {
    	_mtim16_unmask_int(timer);
    }
    
    return MQX_OK;
}

_mqx_int _mtim16_timer_install_kernel
(
		/* [IN] the timer to initialize */
		uint8_t    timer,

		/* [IN] ticks per second */
		uint32_t   tickfreq,

		/* [IN] input clock speed in Hz */
		uint32_t   clk,

	    /* [IN] interrupt priority */
	    uint32_t priority,

		/* [IN] unmask the timer after installation */
		bool   unmask_timer
)
{
    uint32_t result;
    uint32_t period;
    _mqx_uint vector = _bsp_get_mtim16_vector(timer);

    if (vector == 0)
    {
        return MQX_INVALID_DEVICE;
    }

    _bsp_int_disable(vector);

    /* Set up tick timer */
    period = _mtim16_timer_init(timer, tickfreq, clk, FALSE);

    /* Install the timer interrupt handler */
    if (_int_install_isr(vector, _mtim16_kernel_isr, NULL) == NULL)
	{
		return MQX_TIMER_ISR_INSTALL_FAIL;
	}

    /* Initialize the timer interrupt */
    _time_set_timer_vector(_bsp_get_mtim16_vector(timer));
    _time_set_hwtick_function(_mtim16_get_hwticks, (void *) timer);
    _time_set_hwticks_per_tick(period);
    _time_set_ticks_per_sec(tickfreq);

    _bsp_int_init(vector, priority, 0, TRUE);
    
    _bsp_int_enable(vector);

    if (unmask_timer) {
    	_mtim16_unmask_int(timer);
    }

    return MQX_OK;

}

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mtim16_timer_init
* Returned Value  : the clock rate for the timer (ns per hw tick)
* Comments        :
*   this function will set up a timer to interrupt
*
*END*---------------------------------------------------------------------*/

uint32_t _mtim16_timer_init
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
    uint32_t period, prescale = 0;

    volatile MTIM16_STRUCT       *mtim16_ptr;
   
    mtim16_ptr = _bsp_get_mtim16_base_address(timer);
    if (mtim16_ptr == NULL) return 0;

    period = (clk / tickfreq);
    while (period > (0xFFFF+1)) {
        prescale += 1;  /* divides clock in half */
        clk >>= 1;
        period = (clk / tickfreq);
    }

    _bsp_mtim16_clk_en(timer); /* enable clock to the MTIM */

    /* reset and stop counter */
    mtim16_ptr->MTIMSC = MTIM16_SC_TRST_MASK | MTIM16_SC_TSTP_MASK;

    /* set registers */
    mtim16_ptr->MTIMCLK = (uint8_t) prescale;  
    mtim16_ptr->MTIMMOD = (uint16_t) period - 1;

    /* start counter and enable interrupt (if desired) */
    mtim16_ptr->MTIMSC =  (unmask_timer ? MTIM16_SC_TOIE_MASK : 0);

    return period;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mtim16_get_hwticks
* Returned Value  : Number of ticks
* Comments        :
*   This function will return the number of ticks elapsed since the last
* timer tick.
*
*END*---------------------------------------------------------------------*/
uint32_t _mtim16_get_hwticks
   (
      /* [IN] the timer to clear */
      void   *parameter
   )
{
    uint8_t  timer = (uint8_t)parameter;
    uint32_t ticks;
    uint32_t modulo;

    volatile MTIM16_STRUCT       *mtim16_ptr;
   
    mtim16_ptr = _bsp_get_mtim16_base_address(timer);
    if (mtim16_ptr == NULL) return 0;

    modulo = mtim16_ptr->MTIMMOD + 1;
    ticks = mtim16_ptr->MTIMCNT;

    if ((mtim16_ptr->MTIMSC & MTIM16_SC_TOF_MASK) != 0) {
        /* Another full TICK period has expired since we handled the last timer interrupt.
        We need to read the counter again, since the wrap may have
        occurred between the previous read and the checking of the overflow bit. */
        ticks = mtim16_ptr->MTIMCNT + modulo;
    }

    /* actual ticks in MTIMCNT + what we have missed because of sw prescaller */ 
    return ticks;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mtim16_mask_int
* Returned Value  :
* Comments        :
*   This function will mask interrupt of given timer
*
*END*---------------------------------------------------------------------*/

void _mtim16_mask_int
    (
        /* [IN] the timer to use */
        uint8_t    timer
    )
{
    volatile MTIM16_STRUCT       *mtim16_ptr;
   
    mtim16_ptr = _bsp_get_mtim16_base_address(timer);
    if (mtim16_ptr == NULL) return;

    mtim16_ptr->MTIMSC &= ~((uint8_t)MTIM16_SC_TOIE_MASK);
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mtim16_unmask_int
* Returned Value  :
* Comments        :
*   This function will unmask interrupt of given timer
*
*END*---------------------------------------------------------------------*/

void _mtim16_unmask_int
    (
        /* [IN] the timer to use */
        uint8_t    timer
    )
{
    volatile MTIM16_STRUCT       *mtim16_ptr;
   
    mtim16_ptr = _bsp_get_mtim16_base_address(timer);
    if (mtim16_ptr == NULL) return;

    /* clear interrupt flag first, then enable interrupt */
    mtim16_ptr->MTIMSC = (mtim16_ptr->MTIMSC & ~((uint8_t)MTIM16_SC_TOF_MASK)) | MTIM16_SC_TOIE_MASK;
}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _mtim16_clear_int
* Returned Value  :
* Comments        :
*   This function will clear interrupt flag of given timer
*
*END*---------------------------------------------------------------------*/

void _mtim16_clear_int
    (
        /* [IN] the timer to use */
        uint8_t    timer
    )
{
    volatile MTIM16_STRUCT       *mtim16_ptr;
   
    mtim16_ptr = _bsp_get_mtim16_base_address(timer);
    if (mtim16_ptr == NULL) return;

    mtim16_ptr->MTIMSC &= ~((uint8_t)MTIM16_SC_TOF_MASK);
}

static void _mtim16_kernel_isr(void *p) {
	/* Clear timer interrupt flag */
	_mtim16_clear_int(BSP_TIMER);
	_time_notify_kernel();
}
