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
*   This file contains definitions for the programmable interrupt timer
*   utility functions.
*
*
*END************************************************************************/

#ifndef __timer_pit_h__
#define __timer_pit_h__

#include <mqx.h>
#include <bsp.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct pit_timer_init_struct {
   PSP_INTERRUPT_TABLE_INDEX  INTERRUPT;
   _int_level                 LEVEL;
   _int_priority              PRIORITY;
} PIT_TIMER_INIT_STRUCT, * PIT_TIMER_INIT_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*                      FUNCTION PROTOTYPES
*/

extern const PIT_TIMER_INIT_STRUCT _bsp_timers[];

void   *_bsp_get_pit_base_address (uint8_t);
uint32_t _bsp_get_pit_vector (uint8_t);

uint32_t _pit_init_freq(uint8_t, uint32_t, uint32_t, bool);
uint32_t _pit_get_hwticks(void *);
void _pit_clear_int(uint8_t);

#define _pit_unmask_int(timer)  _bsp_int_enable(_bsp_timers[(timer)].INTERRUPT)
#define _pit_mask_int(timer)   _bsp_int_disable(_bsp_timers[(timer)].INTERRUPT)

#ifdef __cplusplus
}
#endif

#endif

