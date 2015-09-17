
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
*   This file contains psp functions for initializing the scheduler.n
*
*
*END************************************************************************/

#include "mqx_inc.h"


/*!
 * \brief This function sets up the kernel disable priority.
 */
void _psp_set_kernel_disable_level
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   MQX_INITIALIZATION_STRUCT_PTR init_ptr;
   uint32_t                       temp;

   _GET_KERNEL_DATA(kernel_data);

   init_ptr    = (MQX_INITIALIZATION_STRUCT_PTR)&kernel_data->INIT;


   /* Compute the enable and disable interrupt values for the kernel. */
   temp = init_ptr->MQX_HARDWARE_INTERRUPT_LEVEL_MAX;
   if ( temp > 7 ) {
      temp = 7;
      init_ptr->MQX_HARDWARE_INTERRUPT_LEVEL_MAX = 7;
   } else if ( temp == 0 ) {
      temp = 1;
      init_ptr->MQX_HARDWARE_INTERRUPT_LEVEL_MAX = 1;
   } /* Endif */

   kernel_data->DISABLE_SR = (uint16_t)0x2000 | (uint16_t)(temp << 8);

} /* Endbody */


/*!
 * \brief This function sets up the kernel priority ready queues
 */
uint32_t _psp_init_readyqs
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   READY_Q_STRUCT_PTR     q_ptr;
   uint32_t                priority_levels;
   uint32_t                n;
   uint16_t                sr;

   _GET_KERNEL_DATA(kernel_data);
   kernel_data->READY_Q_LIST = (READY_Q_STRUCT_PTR) NULL;
   priority_levels = kernel_data->LOWEST_TASK_PRIORITY + 3; // IDLE TASK, INIT TASK

   q_ptr = (READY_Q_STRUCT_PTR)
      _mem_alloc_zero(sizeof(READY_Q_STRUCT) * priority_levels);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if ( q_ptr == NULL ) {
      return (MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(q_ptr, MEM_TYPE_READYQ);

   n = priority_levels;
   while (n--) {
      q_ptr->HEAD_READY_Q  = (TD_STRUCT_PTR)q_ptr;
      q_ptr->TAIL_READY_Q  = (TD_STRUCT_PTR)q_ptr;
      q_ptr->PRIORITY      = (uint16_t)n;
      q_ptr->NEXT_Q        = kernel_data->READY_Q_LIST;
      kernel_data->READY_Q_LIST = q_ptr++;
   } /* Endwhile */

   /* 
   ** Set the current ready q (where the ready queue searches start) to
   ** the head of the list of ready queues.
   */
   kernel_data->CURRENT_READY_Q = kernel_data->READY_Q_LIST;


   /* Initialize the ENABLE_SR fields in the ready queues */
   sr = (uint16_t)0x2000 | 
      (uint16_t)((kernel_data->INIT.MQX_HARDWARE_INTERRUPT_LEVEL_MAX - 1) << 8);
   n  = priority_levels;
   q_ptr =  kernel_data->READY_Q_LIST;
   while (n--) {
      q_ptr->ENABLE_SR = sr;
      if (  ((uint16_t) 0xF00 & sr) > 0  ) {
         sr = (uint16_t)sr - (uint16_t)0x100; 
      } /* Endif */
      q_ptr = q_ptr->NEXT_Q;
   } /* Endwhile */

   return MQX_OK;

} /* Endbody */

/* EOF */
