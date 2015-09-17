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
*    This file contains the boot and boot exception code.
*
*
*END************************************************************************/

#include "mqx_inc.h"
#include "bsp.h"
#include "bsp_prv.h"

/* imported data */
extern unsigned long far _SDA_BASE[];
extern unsigned long far __BOOT_STACK_ADDRESS[];

/* imported routines */
extern void _start(void);

/* exported routines */
asm __declspec(register_abi) void __boot(void);
asm __declspec(register_abi) void __boot_exception(void);


/*  Assume we could start here via the debugger or jump here at any time */
asm __declspec(register_abi) void __boot(void)
{
    /* disable interrupts */
    move.w      #0x2700,sr

    /* setup the stack pointer */
    lea        __BOOT_STACK_ADDRESS, a7

    /* setup A6 dummy stackframe */
    movea.l     #0,a6
    link        a6,#0

    /* setup interrupt vector table */
    move.l #__VECTOR_TABLE_ROM_START, d0
    movec d0, VBR

    /* initialize any hardware specific issues */
    jsr    __init_hardware

    /* jump to the compiler _start function */
    jmp     _start
}


asm __declspec(register_abi) void __boot_exception(void)
{
    halt
    rte
}
