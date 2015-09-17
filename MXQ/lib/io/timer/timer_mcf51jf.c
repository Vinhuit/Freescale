/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
* Copyright 1989-2008 ARC International
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
*   This file contains timer functions for use with a MCF51JF.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "timer_mtim16.h"

#define MTIM16_TIMERS 1

static _mqx_uint _mtim16_vectors[] =
{
	BSP_TIMER_INTERRUPT_VECTOR
};

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_mtim16_vector
* Returned Value   : Vector number upon success, 0 upon failure
* Comments         :
* 	 This function returns interrupt vector number of the corresponding MTIM
*
*END*----------------------------------------------------------------------*/
_mqx_uint _bsp_get_mtim16_vector(uint8_t timer) {
	if (timer > (MTIM16_TIMERS - 1)) {
		return 0;
	} else {
		return _mtim16_vectors[timer];
	}
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_mtim16_base_address
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the base register address of the corresponding MTIM
*
*END*----------------------------------------------------------------------*/
void *_bsp_get_mtim16_base_address (uint8_t timer)
{
    switch (timer) {
        case 0: return (void *)MTIM0_BASE_PTR;
    }
    return NULL;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_mtim16_clk_en
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function enables clock to corresponding MTIM
*
*END*----------------------------------------------------------------------*/
void _bsp_mtim16_clk_en (uint8_t timer)
{
    switch (timer) {
        case 0:
        	SIM_SCGC3_REG(SIM_BASE_PTR) |= SIM_SCGC3_MTIM_MASK;
          break;
    }
}
