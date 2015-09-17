/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
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
*   This file contains definitions for the Coldfire interrupt
*   controller in the MCF5445.
*
*
*END************************************************************************/
#ifndef __int_ctrl_mcf5441_h__
#define __int_ctrl_mcf5441_h__

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*
**                       TYPE DEFINITIONS
**
** The PSP must provide a type or define:
**
**   PSP_INTERRUPT_TABLE_INDEX  - Defines the interrupt table indexes.
**   PSP_INT_FIRST_EXTERNAL     - First external interrupt number used by
**                                this interrupt controller as ICR00.
*/

#define mcf5441_int_priority       _int_level
#define PSP_INTERRUPT_TABLE_INDEX  MCF5441_INTERRUPT_TABLE_INDEX
#define PSP_INT_FIRST_EXTERNAL     MCF5441_INT_FIRST_EXTERNAL

/* Interrupt level */
typedef unsigned char _int_level;

/* Interrupt Acknowledge Level and Priority Register
**
** Interrupt priority can range 0 (lowest) to 7 (highest priority) and
** a special value 8 (mid-point) for fixed level interrupts
*/
typedef unsigned char _int_priority;

#define _interrupt_controller_mask(irq)     _mcf5441_int_mask(irq)
#define _interrupt_controller_unmask(irq)   _mcf5441_int_unmask(irq)

/*****************************************************************************/
/*
**                      FUNCTION PROTOTYPES
*/

uint32_t _mcf5441_int_init(PSP_INTERRUPT_TABLE_INDEX, _int_level, bool unmask);
uint32_t _mcf5441_int_unmask(PSP_INTERRUPT_TABLE_INDEX);
uint32_t _mcf5441_int_mask(PSP_INTERRUPT_TABLE_INDEX);
uint32_t _mcf5441_swint_invoke(_mqx_uint vector);
uint32_t _mcf5441_swint_clear(_mqx_uint vector);
void     _mcf5441_int_mask_all(void);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
