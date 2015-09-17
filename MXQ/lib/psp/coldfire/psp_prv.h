#ifndef __psp_prv_h__
#define __psp_prv_h__
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
*   This file contains psp private declarations for use when compiling
*   the kernel.
*
*
*END************************************************************************/

#define _PSP_GET_CALLING_PC() ((void *)0)

/* This macro modifies the context of a blocked task so that the
** task will execute the provided function when it next runs
*/
#define _PSP_SET_PC_OF_BLOCKED_TASK(stack_ptr, func) \
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->FRAME.RETURN_ADDRESS = \
   (void (_CODE_PTR_)(void))(func)


/* This macro modifies the interrupt priority of a blocked task so that the
** task will execute at the correct interrupt priority when it restarts
*/
#define _PSP_SET_SR_OF_BLOCKED_TASK(stack_ptr, sr_value) \
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->FRAME.STATUS_REGISTER &= ~0700;\
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->FRAME.STATUS_REGISTER |= \
      sr_value;


/* This macro modifies the context of a task that has been interrupted
** so that the task will execute the provided function when the isr returns
*/
#define _PSP_SET_PC_OF_INTERRUPTED_TASK(stack_ptr, func) \
   ((PSP_BASIC_INT_FRAME_STRUCT_PTR)(stack_ptr))->FRAME.RETURN_ADDRESS = \
   (void (_CODE_PTR_)(void))(func)

/* Calculate the address of the td extension found on the stack */
#define _PSP_GET_TD_EXTENSION(td_ptr) \
   ((unsigned char *)((td_ptr)->STACK_BASE) - sizeof(PSP_STACK_START_STRUCT) + \
    FIELD_OFFSET(PSP_STACK_START_STRUCT, TD_EXTENSION))

/*--------------------------------------------------------------------------*/
/*
**                  PROTOTYPES OF PSP FUNCTIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

extern void    _psp_build_float_context(TD_STRUCT_PTR);
extern void   *_psp_get_float_context(void *);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
