
/*HEADER**********************************************************************
*
* Copyright 2012 Freescale Semiconductor, Inc.
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
*   This file contains the function for resetting the pc of interrupted task.
*
*END************************************************************************/

#include "mqx_inc.h"

/* Saved the stack pointers for the various interrupt modes */
extern PSP_BASIC_INT_FRAME_STRUCT_PTR _fiq_sp;
extern PSP_BASIC_INT_FRAME_STRUCT_PTR _undef_sp;
extern PSP_BASIC_INT_FRAME_STRUCT_PTR _abort_sp;

/*
 * \brief This function sets the pc of an interrupted task.
 *
 * \param[in] td_stack_ptr Stack pointer of selected task.
 * \param[in] function     The function to set the pc to.
 */
void _psp_set_pc_of_interrupted_task
    (
        PSP_BASIC_INT_FRAME_STRUCT_PTR td_stack_ptr,
        void (_CODE_PTR_ function)(uint32_t)
    )
{
    PSP_BASIC_INT_FRAME_STRUCT_PTR  stack_ptr;
    KERNEL_DATA_STRUCT_PTR kernel_data;

    _GET_KERNEL_DATA(kernel_data);
    
    switch (__get_CPSR() & PSP_PSR_MODE_MASK) {
    case PSP_PSR_MODE_FIQ:
        stack_ptr = _fiq_sp;
        break;
    case PSP_PSR_MODE_IRQ:
        stack_ptr = kernel_data->INTERRUPT_STACK_PTR;
        break;
    case PSP_PSR_MODE_UNDEF:
        stack_ptr = _undef_sp;
        break;
    case PSP_PSR_MODE_ABORT:
        stack_ptr = _abort_sp;
        break;
    default:
        stack_ptr = td_stack_ptr;
        break;
    }
    
    /*
    * Stack pointer is actually on the top. We need to add the stack
    * frame to it
    */
    stack_ptr--;
    stack_ptr->STACK.PC = (uint32_t)function + 4;         // +4 because return from exception (IRQ)... (subs pc, lr, #4)
}
