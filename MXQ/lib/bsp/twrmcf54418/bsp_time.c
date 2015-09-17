/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   This file contains MCF548x time utility functions
*   $Header:bsp_time.c, 2, 9/9/2004 2:44:36 PM, Goutham D. R.$
*   $NoKeywords$
*
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_elapsed_time
* Returned Value   : none
* Comments         :
*
*END*----------------------------------------------------------------------*/

void _bsp_spin_time
   (
      // [IN] number of microseconds to wait
      uint32_t us
   )
{ /* Body */

   uint32_t inst_per_us = BSP_SYSTEM_CLOCK / 1000000;
   uint32_t inst_to_exec = us * inst_per_us;

   // This is actually too long.  The nop is serializing
   // so we'll only perform 1 then sync the entire pipeline.
   // So if the pipeline is 6 deep then this loop is 6 times
   // longer than the user requested.  The PowerPC had a
   // timebase register that we could really be wall clock
   // accurate with.  We need something like that here.
   for (; inst_to_exec != 0; inst_to_exec--) {
      _ASM_NOP();
   } /* Endfor */

} /* Endbody */

/* EOF */
