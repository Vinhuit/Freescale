/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
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
*   This file contains the type definitions for the mcf522xx PIT module.
*
*
*END************************************************************************/

#ifndef __mcf5xxx_pit_h__
#define __mcf5xxx_pit_h__

#define __mcf5xxx_pit_h__version "$Version:3.6.2.0$"
#define __mcf5xxx_pit_h__date    "$Date:Jun-4-2010$"

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------*/
/*
** PIT  registers bit set.
*/

/* Bit definitions and macros for MCF54XX_PIT_PCSR */


#define MCF5XXX_PIT_PCSR_EN                      (0x1)
#define MCF5XXX_PIT_PCSR_RLD                     (0x2)
#define MCF5XXX_PIT_PCSR_PIF                     (0x4)
#define MCF5XXX_PIT_PCSR_PIE                     (0x8)
#define MCF5XXX_PIT_PCSR_PRESCALE_SHIFT           8
#define MCF5XXX_PIT_PCSR_OVW                     (0x10)
#define MCF5XXX_PIT_PCSR_DBG                     (0x20)
#define MCF5XXX_PIT_PCSR_HALTED                  (1<<5)
#define MCF5XXX_PIT_PCSR_DOZE                    (0x40)
#define MCF5XXX_PIT_PCSR_PRE(x)                  (((x) & 0xF) << 8)

/* Bit definitions and macros for MCF5XXX_PIT_PMR */
#define MCF5XXX_PIT_PMR_PM(x)                    (((x)&0xFFFF) << 0)

/* Bit definitions and macros for MCF5XXX_PIT_PCNTR */
#define MCF5XXX_PIT_PCNTR_PC(x)                  (((x)&0xFFFF) << 0)

/*
** MCF5XXX_PIT_STRUCT                                                                  
** Programmable interrupt timer
*/
typedef struct mcf5xxx_pit_struct
{
   uint16_t  PCSR;           // PIT control and status register
   uint16_t  PMR;            // PIT modulus register
   uint16_t  PCNTR;          // PIT count register
   unsigned char    filler[0x3FFA];
} MCF5XXX_PIT_STRUCT, * MCF5XXX_PIT_STRUCT_PTR;
typedef volatile struct mcf5xxx_pit_struct * VMCF5XXX_PIT_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf5xxx_pit_h__
