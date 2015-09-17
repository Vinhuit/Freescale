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
*   This file contains the type definitions for the mcf522xx PIT module.
*
*
*END************************************************************************/

#ifndef __mcf522xx_pit_h__
#define __mcf522xx_pit_h__

#define __mcf522xx_pit_h__version "$Version:3.6.2.0$"
#define __mcf522xx_pit_h__date    "$Date:Jun-4-2010$"

#ifdef __cplusplus
extern "C" {
#endif

/*
** MCF522XX_PIT_STRUCT                                                                  
** Programmable interrupt timer
*/
typedef struct mcf522xx_pit_struct
{
   uint16_t  PCSR;           // PIT control and status register
   uint16_t  PMR;            // PIT modulus register
   uint16_t  PCNTR;          // PIT count register
   unsigned char    filler[0xFFFA];
} MCF522XX_PIT_STRUCT, * MCF522XX_PIT_STRUCT_PTR;
typedef volatile struct mcf522xx_pit_struct * VMCF522XX_PIT_STRUCT_PTR;

#ifdef __cplusplus
}
#endif

#endif  //__mcf522xx_pit_h__
