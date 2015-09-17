
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
* See license agreement file for full license terms including other restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the function for initializing the handling of 
*   interrupts.
*
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/*!
 * \brief This function initializes kernel interrupt tables. 
 */
void _psp_int_install
   (
      void
   )
{ /* Body */

#if !MQX_ROM_VECTORS
   uint32_t                    i;
   uint32_t                *vbr_ptr;
   
   vbr_ptr = (uint32_t *)_int_get_vector_table();
      
   /* Initialize the hardware interrupt vector table */   
   // 0: Initial stack pointer
   // 1: Initial program counter
   for (i = 2; i < PSP_MAXIMUM_INTERRUPT_VECTORS; i++ ) {
      vbr_ptr[i] = (uint32_t)_int_kernel_isr;
   } /* Endfor */
#endif
} /* Endbody */

#endif
/* EOF */
