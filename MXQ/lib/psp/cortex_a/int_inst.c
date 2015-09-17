
/*HEADER**********************************************************************
*
* Copyright 2010 Freescale Semiconductor, Inc.
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

/* We need to save the stack pointer for the various interrupt modes */
PSP_BASIC_INT_FRAME_STRUCT_PTR _fiq_sp;
PSP_BASIC_INT_FRAME_STRUCT_PTR _undef_sp;
PSP_BASIC_INT_FRAME_STRUCT_PTR _abort_sp;

/*!
 * \brief This function initializes kernel interrupt tables.
 */
void _psp_int_install(void) {
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);
   __set_mode_SP((uint32_t)kernel_data->INTERRUPT_STACK_PTR, (PSP_PSR_MODE_IRQ | PSP_PSR_CTRL_DIS_FIQ | PSP_PSR_CTRL_DIS_IRQ));
   __set_mode_SP((uint32_t)_fiq_sp, (PSP_PSR_MODE_FIQ | PSP_PSR_CTRL_DIS_FIQ | PSP_PSR_CTRL_DIS_IRQ));
   __set_mode_SP((uint32_t)_abort_sp, (PSP_PSR_MODE_ABORT | PSP_PSR_CTRL_DIS_FIQ | PSP_PSR_CTRL_DIS_IRQ));
   __set_mode_SP((uint32_t)_undef_sp, (PSP_PSR_MODE_UNDEF | PSP_PSR_CTRL_DIS_FIQ | PSP_PSR_CTRL_DIS_IRQ));
}
