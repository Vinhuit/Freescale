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
*   This file contains timer functions for use with a MCF5208.
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "timer_pit.h"


const PIT_TIMER_INIT_STRUCT _bsp_timers[MCF5225_NUM_TIMERS] = {
   {MCF5225_INT_PIT0, BSP_PIT0_INT_LEVEL, BSP_PIT0_INT_SUBLEVEL},
   {MCF5225_INT_PIT1, BSP_PIT1_INT_LEVEL, BSP_PIT1_INT_SUBLEVEL}
};


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_pit_base_address
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the base register address of the corresponding PIT
*
*END*----------------------------------------------------------------------*/

void *_bsp_get_pit_base_address (uint8_t timer)
{
  volatile MCF5225_STRUCT      *reg_ptr = _PSP_GET_IPSBAR();
  if (timer >= MCF5225_NUM_TIMERS) return NULL;
  return (void *)&reg_ptr->PIT[timer];
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_pit_vector
* Returned Value   : Vector number upon success, 0 upon failure
* Comments         :
*    This function returns desired interrupt vector number for specified PIT
*
*END*----------------------------------------------------------------------*/

uint32_t _bsp_get_pit_vector (uint8_t timer)
{
  if (timer >= MCF5225_NUM_TIMERS) return 0;
  return _bsp_timers[timer].INTERRUPT;
}
