
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
*   This file contains the isr that handles exceptions.
*
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS
/*!
 * \brief To provide support for exception handlers, applications can use this ISR
 * to replace the default ISR. The ISR is specific to the PSP.
 * 
 * An application calls _int_install_exception_isr() to install _int_exception_isr().
 * \n The function _int_exception_isr() does the following:
 * \li If an exception occurs when a task is running and a task exception ISR 
 * exists, MQX runs the ISR; if a task exception ISR does not exist, MQX aborts 
 * the task by calling _task_abort().
 * \li If an exception occurs when an ISR is running and an ISR exception ISR 
 * exists, MQX aborts the running ISR and runs the ISR's exception ISR.
 * \li The function walks the interrupt stack looking for information about the 
 * ISR or task that was running before the exception occurred. If the function 
 * determines that the interrupt stack contains incorrect information, it calls 
 * _mqx_fatal_error() with error code MQX_CORRUPT_INTERRUPT_STACK.
 * 
 * \param[in] parameter Parameter passed to the default ISR (the vector number).
 * 
 * \warning See description.
 * 
 * \see _int_install_exception_isr
 * \see _mqx_fatal_error
 * \see _task_abort     
 */
void _int_exception_isr
   (
      void   *parameter
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   TD_STRUCT_PTR              td_ptr;
   PSP_INT_CONTEXT_STRUCT_PTR exception_frame_ptr;
   PSP_INT_CONTEXT_STRUCT_PTR isr_frame_ptr;
   INTERRUPT_TABLE_STRUCT_PTR table_ptr;
   void                           (*exception_handler)
      (uint32_t, uint32_t, void *, void *);
   uint32_t                    exception_vector;
   uint32_t                    isr_vector;
   void                     (*func)(void);
   void                      *a6_addr;

   _GET_KERNEL_DATA(kernel_data);
   td_ptr = kernel_data->ACTIVE_PTR;

   /* Stop all interrupts */
   _PSP_SET_DISABLE_SR(kernel_data->DISABLE_SR);

   if ( kernel_data->IN_ISR > 1 ) {
      /* We have entered this function from an exception that happened
       * while an isr was running.
       */

      /* the interrupt context pointer stored in the kernel is part
       * of the interrupt frame structure, whose address can then be determined.
       */
      exception_frame_ptr = (void *)kernel_data->INTERRUPT_CONTEXT_PTR;
      exception_vector    = exception_frame_ptr->EXCEPTION_NUMBER;

      /* the current context contains a pointer to the next one */
      isr_frame_ptr = (void *)
         exception_frame_ptr->PREV_CONTEXT;
      if (isr_frame_ptr == NULL) {
         /* This is not allowable */
         _mqx_fatal_error(MQX_CORRUPT_INTERRUPT_STACK);
      }/* Endif */
      isr_vector    = isr_frame_ptr->EXCEPTION_NUMBER;

      /* Call the isr exception handler for the ISR that WAS running */
      table_ptr = kernel_data->INTERRUPT_TABLE_PTR;
#if MQX_CHECK_ERRORS
      if ((table_ptr != NULL) &&
         (isr_vector >= kernel_data->FIRST_USER_ISR_VECTOR) &&
         (isr_vector <= kernel_data->LAST_USER_ISR_VECTOR))
      {
#endif
      /* Call the exception handler for the isr on isr_vector,
       * passing the isr_vector, the exception_vector, the isr_data and
       * the basic frame pointer for the exception
       */
        exception_handler = _int_get_exception_handler(isr_vector);

//        table_ptr = &table_ptr[isr_vector - kernel_data->FIRST_USER_ISR_VECTOR];
//        exception_handler = table_ptr->APP_ISR_EXCEPTION_HANDLER;
    
        if (exception_handler) {
            (*exception_handler)(isr_vector, (_mqx_uint)parameter, _int_get_isr_data(isr_vector)/*table_ptr->APP_ISR_DATA*/, exception_frame_ptr);
        }
    
#if MQX_CHECK_ERRORS
      } else {
         /* In this case, the exception occured in this handler */
         _mqx_fatal_error(MQX_INVALID_VECTORED_INTERRUPT);
      } /* Endif */
#endif
      
      /* Indicate we have popped 1 interrupt stack frame (the exception frame) */
      --kernel_data->IN_ISR;

      /* Jump to the location in _int_kernel_isr after where the user isr
       * has returned, and before we de-allocate the interrupt frame
       */
      func = _int_kernel_isr_return_internal;

      /* Reset the A6 link register */
      a6_addr = (void *)((unsigned char *)isr_frame_ptr +
         sizeof(PSP_INT_CONTEXT_STRUCT));

#if defined(__CODEWARRIOR__) || defined (__ICCCF__)
      _PSP_SET_A6_SP_AND_GOTO(a6_addr, isr_frame_ptr, func);
#else         

      _PSP_SET_A6(a6_addr);
      
       /* Reset the stack to point to the interrupt frame */
      _PSP_SET_SP(isr_frame_ptr);

       /* And off we go */
      _PSP_GOTO(func);
#endif

   } else {
      /* We have entered this function from an exception that happened
       * while a task was running.
       */
#if MQX_HAS_EXCEPTION_HANDLER
      if (td_ptr->EXCEPTION_HANDLER_PTR != NULL ) {
         (*td_ptr->EXCEPTION_HANDLER_PTR)((uint32_t)parameter, td_ptr->STACK_PTR);      
         return;
      }
#endif
         /* Abort the current task */
         _task_abort(MQX_NULL_TASK_ID);
   }/* Endif */
   
} /* Endbody */

#endif

