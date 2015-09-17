
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
*   This file contains the function for the unexpected
*   exception handling function for MQX, which will display on the
*   console what exception has ocurred.
*   NOTE: the default I/O for the current task is used, since a printf
*   is being done from an ISR.
*   This default I/O must NOT be an interrupt drive I/O channel.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "fio.h"

#if MQX_USE_INTERRUPTS


/*!
 * \brief An MQX-provided default ISR for unhandled interrupts. The function
 * depends on the PSP.
 *
 * The function changes the state of the active task to UNHANDLED_INT_BLOCKED and
 * blocks the task.
 * \n The function uses the default I/O channel to display at least:
 * \li Vector number that caused the unhandled exception.
 * \li Task ID and task descriptor of the active task.
 *
 * \n Depending on the PSP, more information might be displayed.
 *
 * \param[in] parameter Parameter passed to the default ISR.
 *
 * \note
 * Since the ISR uses printf() to display information to the default I/O channel,
 * default I/O must not be on a channel that uses interrupt-driven I/O or the
 * debugger.
 * \warning Blocks the active task.
 *
 * \see _int_install_unexpected_isr
 */
void _int_unexpected_isr
   (
      void   *parameter
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR            kernel_data;
   PSP_BASIC_INT_FRAME_STRUCT_PTR    basic_frame_ptr;
   PSP_INTERRUPT_FRAME_STRUCT_PTR    frame_ptr;
   TD_STRUCT_PTR                     td_ptr;
   volatile uint32_t                           vector, offset;

   _GET_KERNEL_DATA(kernel_data);
   td_ptr      = kernel_data->ACTIVE_PTR;

   if (kernel_data->IN_ISR == 1) {
      /* Get the pointer to the data stored on the task stack */
      basic_frame_ptr = (void *)td_ptr->STACK_PTR;
   } else {
      frame_ptr = (PSP_INTERRUPT_FRAME_STRUCT_PTR)&parameter;
      basic_frame_ptr = &frame_ptr->BASIC_FRAME;
   }/* Endif */

   vector = PSP_FAV_GET_VECTOR_NUMBER(basic_frame_ptr->FRAME.FORMAT_AND_VECTOR);
   offset = PSP_FAV_GET_VECTOR_OFFSET(basic_frame_ptr->FRAME.FORMAT_AND_VECTOR);

   printf( "\n*** UNHANDLED INTERRUPT ***\n");
   printf( "Vector #: %d  0x%x\n\r",vector,vector);
   printf( "Offset  : %d  0x%x\n\r",offset,offset);
   printf( "Task Id: 0x%0x Td_ptr 0x%x Stack Frame: 0x%x\n\r",
      td_ptr->TASK_ID, td_ptr, basic_frame_ptr);

   printf( "Interrupt_nesting level: %d   PC: 0x%08x   SR: 0x%04x\n\r",
      kernel_data->IN_ISR,
      basic_frame_ptr->FRAME.RETURN_ADDRESS,
      basic_frame_ptr->FRAME.STATUS_REGISTER );

   td_ptr->STATE = UNHANDLED_INT_BLOCKED;
   _task_set_error_td_internal(td_ptr, MQX_UNHANDLED_INTERRUPT);

   kernel_data->IN_ISR = 0;
   _task_block();

} /* Endbody */

#endif
/* EOF */
