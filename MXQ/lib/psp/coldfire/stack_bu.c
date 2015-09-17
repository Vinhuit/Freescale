
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
*   This file contains the functions for manipulating the user
*   context on the stack.
*   $Header:stack_bu.c, 5, 3/29/2004 12:03:44 PM, $
*   $NoKeywords$
*
*
*END************************************************************************/

#include "mqx_inc.h"

/*!
 * \brief  This function sets up the stack frame of a new task descriptor.
 * 
 * \param[in] td_ptr the address of the task descriptor
 * \param[in] stack_ptr the address of the stack memory block
 * \param[in] stack_size the size of the stack
 * \param[in] template_ptr the task template address
 * \param[in] status_register the status register to use in creating the task
 * \param[in] create_parameter the task creation parameter
 *
 * \return TRUE if successful
 */
bool _psp_build_stack_frame
   (
      /* [IN] the address of the task descriptor */
      TD_STRUCT_PTR    td_ptr,

      /* [IN] the address of the stack memory block */
      void            *stack_ptr,

      /* [IN] the size of the stack */
      uint32_t          stack_size,

      /* [IN] the task template address */
      TASK_TEMPLATE_STRUCT_PTR template_ptr,

      /* [IN] the status register to use in creating the task */
      uint32_t          status_register,

      /* [IN] the task creation parameter */
      uint32_t          create_parameter
   )
{ /* Body */
   unsigned char *stack_base_ptr;
   PSP_STACK_START_STRUCT_PTR stack_start_ptr;
   uint32_t reg_val;
   bool res = TRUE;

   stack_base_ptr  = (unsigned char *)_GET_STACK_BASE(stack_ptr, stack_size);
   stack_start_ptr = (void *)(stack_base_ptr - sizeof(PSP_STACK_START_STRUCT));

   td_ptr->STACK_BASE  = (void *)stack_base_ptr;
#if MQX_TD_HAS_STACK_LIMIT
   td_ptr->STACK_LIMIT = _GET_STACK_LIMIT(stack_ptr, stack_size);
#endif
   td_ptr->STACK_PTR   = stack_start_ptr;

   /*
   ** Build the task's initial stack frame. This contains the initialized
   ** registers, and an exception frame which will cause the task to
   ** "return" to the start of the task when it is dispatched.
   */
   _mem_zero(stack_start_ptr, (uint32_t)sizeof(PSP_STACK_START_STRUCT));

   stack_start_ptr->INITIAL_CONTEXT.FRAME.STATUS_REGISTER =
      (uint16_t)status_register;

   stack_start_ptr->INITIAL_CONTEXT.FRAME.FORMAT_AND_VECTOR =
      (uint16_t)PSP_NORMAL_STACK_FRAME;

   stack_start_ptr->INITIAL_CONTEXT.FRAME.RETURN_ADDRESS =
      (void (_CODE_PTR_)(void))  template_ptr->TASK_ADDRESS;

   stack_start_ptr->EXIT_ADDRESS    = _task_exit_function_internal;

#if PSP_ABI == PSP_ABI_REG
   stack_start_ptr->INITIAL_CONTEXT.REGISTERS.D0 = create_parameter;
#endif

   stack_start_ptr->PARAMETER       = create_parameter;

   _PSP_GET_A5(reg_val);
   stack_start_ptr->INITIAL_CONTEXT.REGISTERS.A5    = (void *)reg_val;

   /* Mark the bottom of the stack for debuggers*/
   stack_start_ptr->INITIAL_CONTEXT.REGISTERS.A6    =
      &stack_start_ptr->ZERO_LINK_ADDRESS;

#if PSP_HAS_FPU
   if ((td_ptr->FLAGS & MQX_FLOATING_POINT_TASK) != 0) {
      res = _psp_build_float_context(td_ptr);
   } /* Endif */
#endif

   return res;	
} /* Endbody */


#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
/*!
 * \brief This function sets up the FLOATING POINT context of a new task descriptor.
 * 
 * \param[in] td_ptr the address of the task descriptor
 */
bool _psp_build_float_context
   (
      /* [IN] the address of the task descriptor */
      TD_STRUCT_PTR    td_ptr
   )
{ /* Body */
   PSP_BLOCKED_FP_STRUCT_PTR fp_ptr;
   KERNEL_DATA_STRUCT_PTR   kernel_data;

   _GET_KERNEL_DATA(kernel_data);
#if MQX_USE_LWMEM_ALLOCATOR==0 && MQX_USE_TLSF_ALLOCATOR==0
   STOREBLOCK_STRUCT_PTR block_ptr;
#endif

   /* Allocate space for saving/restoring the DSP registers */
   fp_ptr = (PSP_BLOCKED_FP_STRUCT_PTR)_mem_alloc_zero(
      (_mem_size)sizeof(PSP_BLOCKED_FP_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (!fp_ptr) {
      /* Couldn't allocate memory for the DSP register context */
      _task_set_error_td_internal(td_ptr, MQX_OUT_OF_MEMORY);
      return FALSE;
   } /* Endif */
#endif

   _mem_set_type(fp_ptr, MEM_TYPE_FP_CONTEXT);
   _mem_transfer_td_internal(fp_ptr, kernel_data->ACTIVE_PTR, td_ptr);

   /* This field should never be overwitten */
   fp_ptr->TID = td_ptr->TASK_ID;

   td_ptr->FLOAT_CONTEXT_PTR = (void *)fp_ptr;
   
   return TRUE;
} /* Endbody */


/*!
 * \brief This function returns the FLOAT context of a the specified task.
 * 
 * \param[in] in_td_ptr the address of the task descriptor
 *
 * \return pointer to task's FLOATING POINT context
 */
void *_psp_get_float_context
   (
      /* [IN] the address of the task descriptor */
      void      *in_td_ptr
   )
{ /* Body */
   TD_STRUCT_PTR td_ptr;

#if MQX_CHECK_ERRORS
   /* This field should never be overwitten */
   if (!in_td_ptr) {
      return NULL;
   } /* Endif */
#endif

   td_ptr = (TD_STRUCT_PTR)in_td_ptr;

   return td_ptr->FLOAT_CONTEXT_PTR;

} /* Endbody */
#endif
/* EOF */
